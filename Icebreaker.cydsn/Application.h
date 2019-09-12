/*******************************************************************************
* File Name: Application.h
*
* Version 1.0
*
*  Description: Application.h provides various function prototypes for all the
*               Application tasks
*
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

#ifndef APP_H
	#define APP_H

  #include <Features.h>		
	#include "Config.h"
	#include <UART.h>
    #include <UART_SPI_UART.h>
    #include <USBUART_cdc.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <Calibration.h>
    #include <timers.h>
    
    #define FW_VER_MAJOR        1
    #define FW_VER_MINOR        0
    #define FW_VER_REV          0
		
	#ifdef TXDEBUG
        extern char gbuf[];
		#define PRINT	USBUART_PutString
        #define PRINTF(...) \
            do \
            { \
                if (0u != USBUART_GetConfiguration()) \
                { \
                    uint32_t s1 = tmrGetCounter_ms(); \
                    while (0u == USBUART_CDCIsReady() && (tmrGetElapsedMs(s1) < 1000)); \
                    sprintf(&gbuf[0], __VA_ARGS__); \
                    USBUART_PutString(gbuf); \
                } \
            } while (0)
        #define DM_LVL_ERROR    (1 << 0)
        #define DM_LVL_WARNING  (1 << 1)
        #define DM_LVL_INFO     (1 << 2)
        #define DM_LVL_DEBUG    (1 << 3)
        
        #ifdef FEATURE_DEBUG_PRINTS
          #define D_PRINTF(lvl, ...) do { if ((cal_data != NULL) && (DM_LVL_ ## lvl & cal_data->debug_mask)) { PRINTF(__VA_ARGS__); } } while (0)
        #else
          #define D_PRINT(lvl, ...)
        #endif
	#else	
		#define PRINT(x)
        #define PRINTF(...)
        #define D_PRINTF(lvl, ...)
	#endif	/* #ifdef TXDEBUG */

	/*  Function Prototype  */
	void InitApp(void);          /* Configures application clocks and initializes components */
	void RunApplication(void);                /* User Application main routine */
	uint8 Update_VolumeAudioOut(void);

	/* Constants for USB descriptors */
	#define PC_MAC_AUDIO_WITH_VOLUME_DEVICE       0x00
	#define AUDIO_OUT_ENDPOINT                    0x01
	#define AUDIO_CTRL_ENDPOINT                   0x06
	#define AUDIO_IN_ENDPOINT                     0x02
	#define ASYNC_EXPLICIT_FEEDBACK_ENDPOINT      0x03
	#define MAC_PC_HID_CONTROL_ENDPOINT           0x04

	#define VOLUME_MUTE							  0
	#define MIC_MUTE							  1
	#define UP_BUTTON_FUNCTION					  VOLUME_MUTE

	/* Constants for MAC/PC playlist control */
	#define MAC_PC_PLAY_PAUSE_MASK                 0x01
	#define MAC_PC_NEXT_TRACK_MASK                 0x02
	#define MAC_PC_PREVIOUS_TRACK_MASK             0x04
	#define MAC_PC_STOP_MASK			           0x08
	#define MAC_PC_MUTE_MASK             		   0x10
	#define MAC_PC_VOL_UP_MASK             		   0x20
	#define MAC_PC_VOL_DOWN_MASK		           0x40

	#define VOL_CHANGE_UPDATE_FREQ				   50
	#define VOL_CHANGE_THRESHOLD				   6
	#define VOL_ACC_LIMIT						   5

	#define VOLUME_BTN_UPDATE

	#if (VOLUME_CTRL == WINDOWS_7_VOLUME_CTL)
		#define PC_VOLUME_MSB_MAX						127
		#define PC_VOLUME_MSB_MIN						50
		#define PC_VOLUME_MSB_CODEC_OFFSET				0
	#elif (VOLUME_CTRL == WINDOWS_10_VOLUME_CTL)
		#define PC_VOLUME_MSB_MAX						100
		#define PC_VOLUME_MSB_MIN						0
		#define PC_VOLUME_MSB_CODEC_OFFSET				50
	#else
		#define PC_VOLUME_MSB_MAX						255
		#define PC_VOLUME_MSB_MIN						0
		#define PC_VOLUME_MSB_CODEC_OFFSET				128
	#endif
	
	/* Different constants used in code  */
	#define TRUE                                   1
	#define FALSE                                  0
	#define ZERO                                   0
	#define FOREVER                                1
	#define ENABLE                                 1
	#define DISABLE                                0
	#define VALID_MODE                             1
	#define INVALID_MODE                           0

	/* Macro definition for checking if host is connected and aux is not active */
	    
	#define IS_HOST_CONNECTED()           			( IsMacPCConnected())     

	#ifdef CAPSENSE_ENABLED
		#define NEXT_BTN	CapSense_RIGHT__BTN
		#define PREV_BTN	CapSense_LEFT__BTN
		#define PLAY_BTN	CapSense_CENTER__BTN
		#define STOP_BTN	CapSense_DOWN__BTN
		#define MUTE_BTN	CapSense_UP__BTN
	#endif
    
    extern volatile uint32_t sys_tick;
    

#endif /* #ifndef APP_H */


//[] END OF FILE

