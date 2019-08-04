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
#include "VolumeControl.h"
#include <Config.h>
#include <Application.h>
#include <ADC.h>
#include <DMA.h>
#include <Codec.h>
//#include <USBFS.h>
#include <pcm1770.h>

int32 ADC_MIN_READING[vol_ctrl_both] = { -2, -2 };
int32 ADC_MAX_READING[vol_ctrl_both] = { 2047, 2047 };

#define ADC_READING_RANGE(side)       (ADC_MAX_READING[side] - ADC_MIN_READING[side])

/* ADC measurement status and result */
int16   adcSample[vol_ctrl_both] = { 0, 0 };

CY_ISR(DmaIntHandler)
{
    // Do nothing
}

void VolumeControlInit(void)
{
    /* Initialize ADC. Conversion is not enabled yet. */
    ADC_Start();

    /* Configure DMA source and destination locations to transfer from
    * ADC data register to memory variable.
    */
    DMA_Start((void *)ADC_SAR_CHAN0_RESULT_PTR, (void *)adcSample);
    
    DMA_SetInterruptCallback(DmaIntHandler);

    /* Start ADC conversion. */
    ADC_StartConvert();
}

void VolumeControlService(void)
{
    static int32 prevVol[vol_ctrl_both] = { 0, 0 };
    int32 volume[vol_ctrl_both] = { 0, 0 };
    int i;

    for (i = 0; i < vol_ctrl_both; i++)
	{
        // Scale ADC reading to USBFS volume range
        volume[i] = (adcSample[i] - ADC_MIN_READING[i]) * PC_VOLUME_MSB_MAX / ADC_READING_RANGE(i);

        #if 0
        /* Filter volume to be in the expected range */
		if((volume[i] > PC_VOLUME_MSB_MIN) && (volume[i] <= PC_VOLUME_MSB_MAX))
		{
			volume[i] = (uint8)(((volume[i] - PC_VOLUME_MSB_MIN) * CODEC_HP_VOLUME_MAX) / (PC_VOLUME_MSB_MAX - PC_VOLUME_MSB_MIN));	 
		}
		else
		{
			/* Set volume to 0 if the volume from PC is not in expected range */
			volume[i] = 0;
		}
        #endif

        if (volume[i] != prevVol[i])
        {
            /* Update the codec volume */
            D_PRINTF(DEBUG, "%s volume knob change %ld -> %ld\n", (i == vol_ctrl_right) ? "Right" : "Left", prevVol[i], volume[i]);
            
            if (i == vol_ctrl_right)
            {
    		    Codec_AdjustBothHeadphoneVolumeLevel(volume[i]);
            }
            else
            {
                pcm1770_volume_set_level(volume[i]);
            }
            prevVol[i] = volume[i];
        }
    }
}

/* [] END OF FILE */
