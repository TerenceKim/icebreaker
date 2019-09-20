/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <Leds.h>
#include <LedManager.h>
#include <Application.h>
#include <LED_RED_PWM_ISR.h>
#include <LED_BLUE_PWM_ISR.h>
#include <LED_GREEN_PWM_ISR.h>
#include <LED_RED_PWM.h>
#include <LED_BLUE_PWM.h>
#include <LED_GREEN_PWM.h>
#include <stdbool.h>
#include <timers.h>

#define PWM_CLOCK_SPEED               (12000000UL) // 12 MHz
#define PWM_MAX_PERIOD_VAL            (60000UL)
#define PWM_PERIOD_TIME_MS            (1000 / (PWM_CLOCK_SPEED / (PWM_MAX_PERIOD_VAL)))

#define PWM_COMPARE_VAL(color_pct, brightness_pct)  (PWM_MAX_PERIOD_VAL - (color_pct * brightness_pct * PWM_MAX_PERIOD_VAL) / 10000)

typedef enum
{
  LEDS_STATE_idle,
  LEDS_STATE_fade_on,
  LEDS_STATE_on,
  LEDS_STATE_fade_off,
  LEDS_STATE_off
} leds_state_e;

typedef struct
{
    void (* startIsr)(void);
    void (* stopIsr)(void);
    void (* clearIsr)(uint32);
    uint32 isrMask;
    void (* start)(void);
    void (* stop)(void);
    void (* writeCounter)(uint32 count);
    uint32 (* readCounter)(void);
    void (* writePeriod)(uint32 period);
    uint32 (* readPeriod)(void);
    void (* writeCompare)(uint32 compare);
    uint32 (* readCompare)(void);
} pwm_callbacks_s;

static pwm_callbacks_s pwm[LED_CH_MAX] = 
{
  // LED_CH_red
  {
    .startIsr = LED_RED_PWM_ISR_Start,
    .stopIsr = LED_RED_PWM_ISR_Stop,
    .clearIsr = LED_RED_PWM_ClearInterrupt,
    .isrMask = LED_RED_PWM_INTR_MASK_TC,
    .start = LED_RED_PWM_Start,
    .stop = LED_RED_PWM_Stop,
    .writeCounter = LED_RED_PWM_WriteCounter,
    .readCounter = LED_RED_PWM_ReadCounter,
    .writePeriod = LED_RED_PWM_WritePeriod,
    .readPeriod = LED_RED_PWM_ReadPeriod,
    .writeCompare = LED_RED_PWM_WriteCompare,
    .readCompare = LED_RED_PWM_ReadCompare
  },
  // LED_CH_green
  {
    .startIsr = LED_GREEN_PWM_ISR_Start,
    .stopIsr = LED_GREEN_PWM_ISR_Stop,
    .clearIsr = LED_GREEN_PWM_ClearInterrupt,
    .isrMask = LED_GREEN_PWM_INTR_MASK_TC,
    .start = LED_GREEN_PWM_Start,
    .stop = LED_GREEN_PWM_Stop,
    .writeCounter = LED_GREEN_PWM_WriteCounter,
    .readCounter = LED_GREEN_PWM_ReadCounter,
    .writePeriod = LED_GREEN_PWM_WritePeriod,
    .readPeriod = LED_GREEN_PWM_ReadPeriod,
    .writeCompare = LED_GREEN_PWM_WriteCompare,
    .readCompare = LED_GREEN_PWM_ReadCompare
  },
  // LED_CH_blue
  {
    .startIsr = LED_BLUE_PWM_ISR_Start,
    .stopIsr = LED_BLUE_PWM_ISR_Stop,
    .clearIsr = LED_BLUE_PWM_ClearInterrupt,
    .isrMask = LED_BLUE_PWM_INTR_MASK_TC,
    .start = LED_BLUE_PWM_Start,
    .stop = LED_BLUE_PWM_Stop,
    .writeCounter = LED_BLUE_PWM_WriteCounter,
    .readCounter = LED_BLUE_PWM_ReadCounter,
    .writePeriod = LED_BLUE_PWM_WritePeriod,
    .readPeriod = LED_BLUE_PWM_ReadPeriod,
    .writeCompare = LED_BLUE_PWM_WriteCompare,
    .readCompare = LED_BLUE_PWM_ReadCompare
  },
};

typedef struct
{
  uint32_t upSlope;
  uint32_t downSlope;
  uint32_t floor;
  uint32_t ceiling;
  uint32_t onTimeMs;
  uint32_t offTimeMs;
  bool     ready;
} pwm_config_s;

typedef struct
{
  int32_t  delta;
  uint32_t lastEventTime;
  leds_state_e state;
} pwm_intr_vars_s;

typedef struct
{
  pwm_config_s      channelCfg[LED_CH_MAX];
  pwm_intr_vars_s   intr[LED_CH_MAX];
  LED_CH_e          notifier;
} leds_cb_s;

static leds_cb_s ledsCb;

static bool override[LED_CH_MAX];
static int32 change[LED_CH_MAX];

static void ledPwmPrep(const led_seq_item_s *pItem)
{
  uint32_t i;
  uint8_t pct[LED_CH_MAX];
  
  pct[LED_CH_red] = ((pItem->color_code >> 16) & 0xFF) * 100UL / 0xFF;
  pct[LED_CH_green] = ((pItem->color_code >> 8) & 0xFF) * 100UL / 0xFF;
  pct[LED_CH_blue] = (pItem->color_code & 0xFF) * 100UL / 0xFF;

  if (pItem->action == LED_ACTION_none)
  {
    // Do nothing
    ledSetEvents(LED_EVENTS_SEQ_CONTINUE);
    return;
  }

  for (i = 0; i < LED_CH_MAX; i++)
  {
    if (pct[i])
    {
      ledsCb.channelCfg[i].ceiling = PWM_COMPARE_VAL(pct[i], pItem->min_brightness_pct);
      ledsCb.channelCfg[i].floor   = PWM_COMPARE_VAL(pct[i], pItem->max_brightness_pct) + 1; // +1 for PWM compare purposes
      ledsCb.channelCfg[i].downSlope = (pItem->fade_on_time_ms < PWM_PERIOD_TIME_MS) ? 0 : (ledsCb.channelCfg[i].ceiling - ledsCb.channelCfg[i].floor) / (pItem->fade_on_time_ms / PWM_PERIOD_TIME_MS);
      ledsCb.channelCfg[i].upSlope = (pItem->fade_off_time_ms < PWM_PERIOD_TIME_MS) ? 0 : (ledsCb.channelCfg[i].ceiling - ledsCb.channelCfg[i].floor) / (pItem->fade_off_time_ms / PWM_PERIOD_TIME_MS);
      ledsCb.channelCfg[i].onTimeMs = pItem->on_time_ms;
      ledsCb.channelCfg[i].offTimeMs = pItem->off_time_ms;
      ledsCb.channelCfg[i].ready = true;
      D_PRINTF(INFO, "[%d].ceiling\t= %d\n", i, ledsCb.channelCfg[i].ceiling);
      D_PRINTF(INFO, "[%d].floor\t= %d\n", i, ledsCb.channelCfg[i].floor);
      D_PRINTF(INFO, "[%d].downSlope\t= %d\n", i, ledsCb.channelCfg[i].downSlope);
      D_PRINTF(INFO, "[%d].upSlope\t= %d\n", i, ledsCb.channelCfg[i].upSlope);
      D_PRINTF(INFO, "[%d].onTimeMs\t= %d\n", i, ledsCb.channelCfg[i].onTimeMs);
      D_PRINTF(INFO, "[%d].offTimeMs\t= %d\n", i, ledsCb.channelCfg[i].offTimeMs);
      ledsCb.notifier = (pItem->iterations == 0) ? LED_CH_none : i;
    }
    else
    {
      memset((void *)&ledsCb.channelCfg[i], 0, sizeof(pwm_config_s));
    }
  }
}

static void ledSetState(LED_CH_e idx, leds_state_e state)
{
  ledsCb.intr[idx].state = state;
}

static leds_state_e ledGetState(LED_CH_e idx)
{
  return ledsCb.intr[idx].state;
}

static void ledPwmStart(void)
{
  uint32_t i;
  
  for (i = 0; i < LED_CH_MAX; i++)
  {
    if (ledsCb.channelCfg[i].ready)
    {
      pwm[i].startIsr();
      pwm[i].start();
      pwm[i].writePeriod(PWM_MAX_PERIOD_VAL);
      pwm[i].writeCounter(1);
      pwm[i].writeCompare(ledsCb.channelCfg[i].ceiling);
      
      ledsCb.intr[i].delta = -ledsCb.channelCfg[i].downSlope;
      
      ledSetState(i, LEDS_STATE_fade_on);
    }
  }
}

static void ledPwmStop(void)
{
  uint32_t i;
  
  for (i = 0; i < LED_CH_MAX; i++)
  {
    pwm[i].stop();
    pwm[i].stopIsr();
  }
}

void LedPlaySeqItem(const led_seq_item_s *pItem)
{
  if (pItem->action == LED_ACTION_none)
  {
    ledPwmStop();
    return;
  }
  
  ledPwmPrep(pItem);
  ledPwmStart();
}

void LedManagerStartOverride(LED_CH_e ledIdx, uint32_t pwmPeriod, uint32_t pwmCompare, uint32_t pwmCounter)
{
    override[ledIdx] = true;

    pwm[ledIdx].startIsr();
    pwm[ledIdx].start();
    pwm[ledIdx].writePeriod(pwmPeriod);
    pwm[ledIdx].writeCounter(pwmCounter);
    pwm[ledIdx].writeCompare(pwmCompare);
    
    change[ledIdx] = pwmCompare;
}

void LedManagerStopOverride(LED_CH_e ledIdx)
{
    pwm[ledIdx].stop();
    override[ledIdx] = false;
}

void LedManagerInterruptHandler(LED_CH_e ledIdx)
{
  int32_t nextVal = (pwm[ledIdx].readCompare() + ledsCb.intr[ledIdx].delta);

  pwm[ledIdx].clearIsr(pwm[ledIdx].isrMask);
  
  if (ledsCb.channelCfg[ledIdx].ready == false)
  {
    return;
  }
  
  switch (ledGetState(ledIdx))
  {
    case LEDS_STATE_fade_on:
    {
      if ((nextVal <= (int32_t)ledsCb.channelCfg[ledIdx].floor) || (ledsCb.intr[ledIdx].delta == 0))
      {
        ledsCb.intr[ledIdx].delta = 0;
        ledsCb.intr[ledIdx].lastEventTime = tmrGetCounter_ms();
        if (ledsCb.intr[ledIdx].delta == 0)
        {
          pwm[ledIdx].writeCompare(ledsCb.channelCfg[ledIdx].floor);
        }
        ledSetState(ledIdx, LEDS_STATE_on);
      }
    } break;
    
    case LEDS_STATE_on:
    {
      if (tmrGetElapsedMs(ledsCb.intr[ledIdx].lastEventTime) >= ledsCb.channelCfg[ledIdx].onTimeMs)
      {
        ledsCb.intr[ledIdx].delta = ledsCb.channelCfg[ledIdx].upSlope;
        ledSetState(ledIdx, LEDS_STATE_fade_off);
      }
    } break;
    
    case LEDS_STATE_fade_off:
    {
      if ((nextVal >= (int32_t)ledsCb.channelCfg[ledIdx].ceiling) || (ledsCb.intr[ledIdx].delta == 0))
      {
        ledsCb.intr[ledIdx].delta = 0;
        ledsCb.intr[ledIdx].lastEventTime = tmrGetCounter_ms();
        if (ledsCb.intr[ledIdx].delta == 0)
        {
          pwm[ledIdx].writeCompare(ledsCb.channelCfg[ledIdx].ceiling);
        }
        ledSetState(ledIdx, LEDS_STATE_off);
        return;
      }
    } break;
    
    case LEDS_STATE_off:
    {
      if (tmrGetElapsedMs(ledsCb.intr[ledIdx].lastEventTime) >= ledsCb.channelCfg[ledIdx].offTimeMs)
      {
        if (ledsCb.notifier != LED_CH_none)
        {
          // We are done here - onto the next item in the sequence (LedManager)
          ledSetEvents(LED_EVENTS_SEQ_CONTINUE);
          ledSetState(ledIdx, LEDS_STATE_idle);
        }
        else
        {
          ledsCb.intr[ledIdx].delta = -ledsCb.channelCfg[ledIdx].downSlope;
          ledSetState(ledIdx, LEDS_STATE_fade_on);
        }
      }
    } break;
    
    case LEDS_STATE_idle:
    default:
      return;
  }
  
  pwm[ledIdx].writeCompare(pwm[ledIdx].readCompare() + ledsCb.intr[ledIdx].delta);
}

/* [] END OF FILE */
