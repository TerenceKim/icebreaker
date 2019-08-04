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
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <UART.h>
#include <VolumeControl.h>
    
typedef struct
{
    uint32 signature;
    uint32 counter;
    int16  adc_min[2];
    int16  adc_max[2];

    uint8  led_max_brightness;
    uint8  debug_mask;
} calibration_data_s;

extern calibration_data_s *cal_data;

void CalibrationInit(void);
void CalibrationRun(void);
bool CalibrationSave(void);
    
#endif /* CALIBRATION_H */

/* [] END OF FILE */
