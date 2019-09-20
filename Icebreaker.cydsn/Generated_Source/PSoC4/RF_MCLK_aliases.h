/*******************************************************************************
* File Name: RF_MCLK.h  
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

#if !defined(CY_PINS_RF_MCLK_ALIASES_H) /* Pins RF_MCLK_ALIASES_H */
#define CY_PINS_RF_MCLK_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RF_MCLK_0			(RF_MCLK__0__PC)
#define RF_MCLK_0_PS		(RF_MCLK__0__PS)
#define RF_MCLK_0_PC		(RF_MCLK__0__PC)
#define RF_MCLK_0_DR		(RF_MCLK__0__DR)
#define RF_MCLK_0_SHIFT	(RF_MCLK__0__SHIFT)
#define RF_MCLK_0_INTR	((uint16)((uint16)0x0003u << (RF_MCLK__0__SHIFT*2u)))

#define RF_MCLK_INTR_ALL	 ((uint16)(RF_MCLK_0_INTR))


#endif /* End Pins RF_MCLK_ALIASES_H */


/* [] END OF FILE */
