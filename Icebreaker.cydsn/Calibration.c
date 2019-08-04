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
#include <Calibration.h>
#include <CyFlash.h>

#define CALIB_SIGN              0xDEADBEEF

/* Defines second ROW from the last ROW */
#define CY_TEST_FLASH_ROW       (CY_FLASH_NUMBER_ROWS - 2u)

/* Defines absolut address of ROW */
#define CY_TEST_FLASH_ADDR      (CY_TEST_FLASH_ROW * CY_FLASH_SIZEOF_ROW)

static uint8 page[CY_FLASH_SIZEOF_ROW];

calibration_data_s *cal_data;

void CalibrationInit(void)
{
    calibration_data_s *pCal = (calibration_data_s *)(CY_TEST_FLASH_ADDR);
    cal_data = (calibration_data_s *)page;
    
    if (pCal->signature == CALIB_SIGN)
    {
        memcpy((void *)cal_data, (void *)pCal, sizeof(calibration_data_s));
    }
    else
    {
        cal_data->signature = CALIB_SIGN;
        
        cal_data->counter = 1;

        cal_data->adc_min[vol_ctrl_right] = -2;
        cal_data->adc_min[vol_ctrl_left] = -2;

        cal_data->adc_max[vol_ctrl_right] = 2047;
        cal_data->adc_max[vol_ctrl_left] = 2047;

        cal_data->led_max_brightness = 50;
        
        cal_data->debug_mask = 0xFF;
    }
}

void CalibrationRun(void)
{
}

bool CalibrationSave(void)
{
    cystatus returnValue;

    cal_data->counter++;

    returnValue = CySysFlashWriteRow(CY_TEST_FLASH_ROW, (uint8 *)cal_data);

    /* Check if operation is successful */
    return (returnValue == CY_SYS_FLASH_SUCCESS);
}

/* [] END OF FILE */
