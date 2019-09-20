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
#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

typedef enum
{
  LED_CH_red,
  LED_CH_green,
  LED_CH_blue,
  LED_CH_none,
  LED_CH_MAX = LED_CH_none
} LED_CH_e;

typedef enum
{
  LED_ACTION_on,
  LED_ACTION_off,
  LED_ACTION_fade_on,
  LED_ACTION_fade_off,
  LED_ACTION_pulse,
  LED_ACTION_blink,
  LED_ACTION_none,
  LED_ACTION_MAX = LED_ACTION_none
} led_action_e;

typedef struct
{
  led_action_e action;
  uint32_t     color_code; /**< RGB code: 0x00RRGGBB. 0xFF000000 means show white/red for battery level. */
  uint16_t     fade_on_time_ms;
  uint16_t     on_time_ms;
  uint16_t     fade_off_time_ms;
  uint16_t     off_time_ms;
  uint8_t      min_brightness_pct;
  uint8_t      max_brightness_pct;
  uint8_t      iterations; /**< Iteration of 0 means infinite */
} led_seq_item_s;

void LedPlaySeqItem(const led_seq_item_s *pItem);

void LedManagerStartOverride(LED_CH_e ledIdx, uint32_t pwmPeriod, uint32_t pwmCompare, uint32_t pwmCounter);
void LedManagerStopOverride(LED_CH_e ledIdx);

void LedManagerInterruptHandler(LED_CH_e ledIdx);

#endif /* LEDS_H */
/* [] END OF FILE */
