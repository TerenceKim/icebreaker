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

typedef enum
{
    LM_LED_RED,
    LM_LED_GREEN,
    LM_LED_BLUE,
    LM_LED_MAX
} LM_LED_e;

void LedManagerStartOverride(LM_LED_e ledIdx, uint32_t pwmPeriod, uint32_t pwmCompare, uint32_t pwmCounter);
void LedManagerStopOverride(LM_LED_e ledIdx);

void LedManagerInterruptHandler(LM_LED_e ledIdx);

#endif /* LED_MANAGER_H */
/* [] END OF FILE */
