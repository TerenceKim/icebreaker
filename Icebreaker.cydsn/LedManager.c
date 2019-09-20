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
#include <LedManager.h>
#include <Leds.h>
#include <LED_RED_PWM_ISR.h>
#include <LED_BLUE_PWM_ISR.h>
#include <LED_GREEN_PWM_ISR.h>
#include <LED_RED_PWM.h>
#include <LED_BLUE_PWM.h>
#include <LED_GREEN_PWM.h>
#include <stdbool.h>

#define LED_MANAGER_MAX_SEQ           (5)

typedef struct
{
  LED_SEQ_e             currentSeq;
  LED_SEQ_e             nextSeq;
  uint8_t               currentSeqIdx;
  uint8_t               iteration;
} led_manager_cb_s;

static const led_seq_item_s ledSequences[LED_SEQ_MAX+1][LED_MANAGER_MAX_SEQ] =
{
  { // LED_SEQ_event_power_on
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations
    {  LED_ACTION_fade_on,  0x000000FF,  250,              3000,        1000,              0,            0,                   100,                 1          },
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0          }
  },
  { // LED_SEQ_event_power_off
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations
    {  LED_ACTION_fade_off, 0x000000FF,  300,              300,         2000,              0,            0,                   100,                 1          },
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0          }
  },
  [ LED_SEQ_none ] =
  {
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0          }
  }
};

static led_manager_cb_s ledManagerCb;
volatile uint32_t ledEvents;

static void ledManagerSequencer(void)
{
  const led_seq_item_s *pCurrentSeqItem = &ledSequences[ledManagerCb.currentSeq][ledManagerCb.currentSeqIdx];
  
  if (pCurrentSeqItem->action == LED_ACTION_none)
  {
    ledSetEvents(LED_EVENTS_SEQ_DONE);
    return;
  }

  LedPlaySeqItem(pCurrentSeqItem);
  
  ledManagerCb.currentSeqIdx++;
}

void LedManagerInit(void)
{
  ledEvents = 0;
  ledManagerCb.currentSeq = LED_SEQ_none;
  ledManagerCb.nextSeq = LED_SEQ_none;
}

void LedManagerService(void)
{
  if (ledCheckEvents(LED_EVENTS_SEQ_CONTINUE))
  {
    ledClearEvents(LED_EVENTS_SEQ_CONTINUE);
    ledManagerSequencer();
  }

  if (ledCheckEvents(LED_EVENTS_SEQ_DONE))
  {
    ledClearEvents(LED_EVENTS_SEQ_DONE);
    ledManagerCb.currentSeq = LED_SEQ_none;
    
    if (ledManagerCb.nextSeq != LED_SEQ_none)
    {
      LedManagerSeqPlay(ledManagerCb.nextSeq, true);
      ledManagerCb.nextSeq = LED_SEQ_none;
    }
  }
}

void LedManagerSeqPlay(LED_SEQ_e seq, bool now)
{
  if (now)
  {
    ledManagerCb.currentSeq = seq;
    ledManagerCb.currentSeqIdx = 0;
    ledManagerSequencer();
  }
  else
  {
    ledManagerCb.nextSeq = seq;
  }
}

LED_SEQ_e LedManagerGetPlayingSeq(void)
{
  return ledManagerCb.currentSeq;
}

/* [] END OF FILE */
