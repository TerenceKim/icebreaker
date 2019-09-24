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
#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <Leds.h>

typedef enum
{
  LED_SEQ_events,
  LED_SEQ_event_power_on = LED_SEQ_events,
  LED_SEQ_event_power_off,
  LED_SEQ_event_nwk_joined,
  LED_SEQ_event_switch_role,
  LED_SEQ_event_show_level,
  LED_SEQ_states,
  LED_SEQ_state_charging = LED_SEQ_states,
  LED_SEQ_state_charged,
  LED_SEQ_state_auto_conn,
  LED_SEQ_state_scanning,
  LED_SEQ_state_connectable,
  LED_SEQ_none,
  LED_SEQ_MAX = LED_SEQ_none
} LED_SEQ_e;

#define LED_EVENTS_SEQ_CONTINUE           (1 <<  0)
#define LED_EVENTS_SEQ_DONE               (1 <<  1)
  
extern volatile uint32_t ledEvents;
  
#define ledSetEvents(e)     { ledEvents |= (e); }
#define ledCheckEvents(e)   ((ledEvents & (e)) != 0)
#define ledClearEvents(e)   { ledEvents &= ~(e); }

void LedManagerInit(void);
void LedManagerService(void);

void LedManagerSeqPlay(LED_SEQ_e seq);
LED_SEQ_e LedManagerGetPlayingSeq(void);
LED_SEQ_e LedManagerGetQueuedSeq(void);

#endif /* LED_MANAGER_H */
/* [] END OF FILE */
