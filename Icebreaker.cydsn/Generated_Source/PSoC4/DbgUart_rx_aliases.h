/*******************************************************************************
* File Name: DbgUart_rx.h  
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

#if !defined(CY_PINS_DbgUart_rx_ALIASES_H) /* Pins DbgUart_rx_ALIASES_H */
#define CY_PINS_DbgUart_rx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DbgUart_rx_0			(DbgUart_rx__0__PC)
#define DbgUart_rx_0_PS		(DbgUart_rx__0__PS)
#define DbgUart_rx_0_PC		(DbgUart_rx__0__PC)
#define DbgUart_rx_0_DR		(DbgUart_rx__0__DR)
#define DbgUart_rx_0_SHIFT	(DbgUart_rx__0__SHIFT)
#define DbgUart_rx_0_INTR	((uint16)((uint16)0x0003u << (DbgUart_rx__0__SHIFT*2u)))

#define DbgUart_rx_INTR_ALL	 ((uint16)(DbgUart_rx_0_INTR))


#endif /* End Pins DbgUart_rx_ALIASES_H */


/* [] END OF FILE */
