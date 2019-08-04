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
#include <LED.h>
#include <LED_BLUE.h>
#include <Application.h>
#include <Calibration.h>

uint32_t pwm_period = 20; // in tenths of milliseconds

bool led_initialized = false;

void LedInit(void)
{
    led_initialized = true;
}

void LedDeInit(void)
{
    led_initialized = false;
}

void LedIntHandler(void)
{
    if (led_initialized)
    {
        if ((sys_tick % pwm_period) == 0)
        {
            LED_BLUE_Write(0);
        }
        else if ((sys_tick % pwm_period) == (cal_data->led_max_brightness * pwm_period / 100))
        {
            LED_BLUE_Write(1);
        }
    }
}

/* [] END OF FILE */
