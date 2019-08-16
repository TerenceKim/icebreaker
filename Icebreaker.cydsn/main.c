/******************************************************************************
* Project Name		: CY8CKIT_046_USB_Audio
* Version			: 1.0
* Device Used		: CY8C4248BZI-L489     
* Software Used		: PSoC Creator 4.2
* Compiler Used		: ARM GCC 5.4.1
* Related Hardware	: CY8CKIT-046 PSoC 4 L-Series Pioneer Kit 
*******************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/
/*******************************************************************************
* Theory of Operation: This project demonstrates the capability of PSoC 4200L to 
* provide a high-quality audio playback and recording interface to a Windows or 
* Mac PC. The example uses the PSoC 4200L USB to implement an asynchronous USB 
* Audio Class v1.0 compliant device. The PSoC 4 L-Series Pioneer Kit has a 
* standard 3.5-mm TRRS audio jack onboard and an audio codec to convert digital 
* audio stream into analog and vice-versa. These components along with PSoC 4200L
* implement the complete USB Audio interface.
*******************************************************************************/

#include <project.h>
#include "Application.h"
#include "Codec.h"
#include "AudioControl.h"
#include "USBInterface.h"
#include <LED.h>
#include <LedManager.h>
#include <timers.h>

CY_ISR(HFI_Handler)
{
  while (1)
  {
    LedManagerStartOverride(LM_LED_BLUE, 63000, 30000, 0);
    LedManagerStartOverride(LM_LED_RED, 63000, 30000, 0);
    LedManagerStartOverride(LM_LED_GREEN, 63000, 30000, 0);
    
    CyDelay(1000);
    
    LedManagerStopOverride(LM_LED_RED);
    LedManagerStopOverride(LM_LED_GREEN);
    LedManagerStopOverride(LM_LED_BLUE);
    
    CyDelay(1000);
  }
}

CY_ISR(SysTickHandler)
{
    tmrService();
}

int main()
{
	//int32 usbEnumFlag = 0;
    
    CyIntSetSysVector(CY_INT_HARD_FAULT_IRQN, HFI_Handler);

    /* Start SysTick */
    CySysTickStart();
    CySysTickSetCallback(0, SysTickHandler);
	
	/* Setup the Audio pipe from USB to I2S.*/	
    //InitAudioPath();
	
	/* Setup the Application part of the firmware */
    InitApp();
	
	PRINTF("InitApp Complete\r\n");
	
    for(;;)
    {
#if 0
        /* Check for USB enumeration with USB host */
        if(USBFS_VBusPresent() == TRUE)
		{
			if( USBFS_GetConfiguration() != FALSE)
	        {				
	            /* Accessory initialization routine after USB enumeration completion 
				 * Also loads the EP's when audio playback/recording starts */
				ServiceUSB();               
	            
	            /* USB audio sampling frequency change handler. */
	            HandleSamplingFrequencyChangeRequest(); 
				
				if(usbEnumFlag == FALSE)
				{
					usbEnumFlag = TRUE; 
					PRINTF("USB Enumeration Done\r\n");
				}
	        }	
		}
		else
		{		
			/* Do not operate the device when VBUS is absent */			
			PRINTF("USB Disconnected \r\n");	
			
			/* Wait till the USB is connected again */
			while(USBFS_VBusPresent() == FALSE);
			
			/* Do a software reset once VBUS is connected to properly reinitialize USB, DMA and I2S components */
			PRINTF("USB Reconnected - SW reset \r\n");
            #ifdef TXDEBUG
			    while(UART_SpiUartGetTxBufferSize() != 0);
            #endif
            
            /* Do a software reset once data is sent over UART */
			CySoftwareReset();		
		}
        
		/* Clear audio IN buffer when IN stream is stopped */
		HandleAudioInBuffer();             
#endif
		/* Runs application layer code for CapSense */
		RunApplication(); 
    }
}

/* [] END OF FILE */
