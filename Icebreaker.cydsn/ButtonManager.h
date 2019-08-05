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

#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H
    
#include <stdio.h>

#define BUTTON_EVENTS_LL_PRESS              (1u << 0)
#define BUTTON_EVENTS_LL_RELEASE            (1u << 1)
#define BUTTON_EVENTS_HOLD_TIMEOUT          (1u << 2)

extern volatile uint32_t buttonEvents;

#define buttonSetEvents(e)     { buttonEvents |= (e); }
#define buttonCheckEvents(e)   ((buttonEvents & (e)) != 0)
#define buttonClearEvents(e)   { buttonEvents &= ~(e); } 

void buttonManagerInit(void);
void buttonManagerService(void);

#endif /* #ifndef BUTTON_MANAGER_H */


//[] END OF FILE

