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
#ifndef LED_H
#define LED_H
    
#include <stdbool.h>
#include <stdint.h>

extern uint32_t pwm_period;
    
extern bool led_initialized;

void LedInit(void);
void LedDeInit(void);
void LedIntHandler(void);
    
#endif /* LED_H */

/* [] END OF FILE */
