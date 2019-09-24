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
#include <Application.h>
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
  [LED_SEQ_event_power_on] = 
  {
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations | immediate
    {  LED_ACTION_fade_on,  0x008000FF,  250,              1000,        0,                 0,            0,                   100,                 1,           true          },
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false         }
  },
  [LED_SEQ_event_power_off] = 
  {
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations | immediate
    {  LED_ACTION_fade_off, 0x008000FF,  300,              300,         1000,              0,            0,                   100,                 1,           true},
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false}
  },
  [LED_SEQ_event_nwk_joined] = 
  {
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations | immediate
    {  LED_ACTION_off,      0x008000FF,  0,                250,         0,                 250,          0,                   100,                 2,           false},
    {  LED_ACTION_fade_off, 0x008000FF,  0,                3000,        1000,              250,          0,                   100,                 1,           false},
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false}
  },
  [LED_SEQ_event_show_level] =
  {
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations | immediate
    {  LED_ACTION_fade_on,  0xFF8000FF,  250,              3000,        1000,              0,            0,                   100,                 1,           false},
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false}
  },
  [LED_SEQ_state_auto_conn] =
  {
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations | immediate
    {  LED_ACTION_on,       0x008000FF,  0,                1000,        0,                 0,            0,                   100,                 0,           false},
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false}
  },
  [LED_SEQ_state_scanning] =
  {
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations | immediate
    {  LED_ACTION_on,       0x008000FF,  0,                0,           500,               0,            0,                   100,                 1,           false},
    {  LED_ACTION_pulse,    0x008000FF,  500,              0,           500,               0,            0,                   100,                 0,           false},
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false}
  },
  [LED_SEQ_state_connectable] =
  {
    // action             | color_code | fade_on_time_ms | on_time_ms | fade_off_time_ms | off_time_ms | min_brightness_pct | max_brightness_pct | iterations | immediate
    {  LED_ACTION_pulse,    0xFF0000FF,  1500,             0,           1500,              0,            20,                  80,                  0,           false},
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false}
  },
  [LED_SEQ_none] =
  {
    {  LED_ACTION_none,     0,           0,                0,           0,                 0,            0,                   0,                   0,           false}
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

  D_PRINTF(INFO, "LED: Playing LED sequence %d, idx %d\n", ledManagerCb.currentSeq, ledManagerCb.currentSeqIdx);
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
    D_PRINTF(INFO, "LED_EVENTS_SEQ_CONTINUE\n");
    ledManagerSequencer();
  }

  if (ledCheckEvents(LED_EVENTS_SEQ_DONE))
  {
    ledClearEvents(LED_EVENTS_SEQ_DONE);
    D_PRINTF(INFO, "LED_EVENTS_SEQ_DONE\n");
    
    ledManagerCb.currentSeq = LED_SEQ_none;
    
    if (ledManagerCb.nextSeq != LED_SEQ_none)
    {
      LedManagerSeqPlay(ledManagerCb.nextSeq);
      ledManagerCb.nextSeq = LED_SEQ_none;
    }
  }
}

void LedManagerSeqPlay(LED_SEQ_e seq)
{
  if (LedManagerGetPlayingSeq() != LED_SEQ_none)
  {
    // Queued to play at next period of current animation
    D_PRINTF(INFO, "LED: Queueing LED sequence %d\n", seq);
    ledManagerCb.nextSeq = seq;
  }
  else
  {
    ledManagerCb.currentSeq = seq;
    ledManagerCb.currentSeqIdx = 0;
    ledManagerSequencer();
  }
}

LED_SEQ_e LedManagerGetPlayingSeq(void)
{
  return ledManagerCb.currentSeq;
}

LED_SEQ_e LedManagerGetQueuedSeq(void)
{
  return ledManagerCb.nextSeq;
}

/* [] END OF FILE */
