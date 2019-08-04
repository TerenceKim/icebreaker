/*******************************************************************************
* File Name: UART_RX_PU.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_UART_RX_PU_ALIASES_H) /* Pins UART_RX_PU_ALIASES_H */
#define CY_PINS_UART_RX_PU_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define UART_RX_PU_0			(UART_RX_PU__0__PC)
#define UART_RX_PU_0_PS		(UART_RX_PU__0__PS)
#define UART_RX_PU_0_PC		(UART_RX_PU__0__PC)
#define UART_RX_PU_0_DR		(UART_RX_PU__0__DR)
#define UART_RX_PU_0_SHIFT	(UART_RX_PU__0__SHIFT)
#define UART_RX_PU_0_INTR	((uint16)((uint16)0x0003u << (UART_RX_PU__0__SHIFT*2u)))

#define UART_RX_PU_INTR_ALL	 ((uint16)(UART_RX_PU_0_INTR))


#endif /* End Pins UART_RX_PU_ALIASES_H */


/* [] END OF FILE */
