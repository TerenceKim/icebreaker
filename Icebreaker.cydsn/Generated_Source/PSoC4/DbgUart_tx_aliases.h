/*******************************************************************************
* File Name: DbgUart_tx.h  
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

#if !defined(CY_PINS_DbgUart_tx_ALIASES_H) /* Pins DbgUart_tx_ALIASES_H */
#define CY_PINS_DbgUart_tx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DbgUart_tx_0			(DbgUart_tx__0__PC)
#define DbgUart_tx_0_PS		(DbgUart_tx__0__PS)
#define DbgUart_tx_0_PC		(DbgUart_tx__0__PC)
#define DbgUart_tx_0_DR		(DbgUart_tx__0__DR)
#define DbgUart_tx_0_SHIFT	(DbgUart_tx__0__SHIFT)
#define DbgUart_tx_0_INTR	((uint16)((uint16)0x0003u << (DbgUart_tx__0__SHIFT*2u)))

#define DbgUart_tx_INTR_ALL	 ((uint16)(DbgUart_tx_0_INTR))


#endif /* End Pins DbgUart_tx_ALIASES_H */


/* [] END OF FILE */
