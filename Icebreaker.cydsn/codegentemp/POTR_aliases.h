/*******************************************************************************
* File Name: POTR.h  
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

#if !defined(CY_PINS_POTR_ALIASES_H) /* Pins POTR_ALIASES_H */
#define CY_PINS_POTR_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define POTR_0			(POTR__0__PC)
#define POTR_0_PS		(POTR__0__PS)
#define POTR_0_PC		(POTR__0__PC)
#define POTR_0_DR		(POTR__0__DR)
#define POTR_0_SHIFT	(POTR__0__SHIFT)
#define POTR_0_INTR	((uint16)((uint16)0x0003u << (POTR__0__SHIFT*2u)))

#define POTR_INTR_ALL	 ((uint16)(POTR_0_INTR))


#endif /* End Pins POTR_ALIASES_H */


/* [] END OF FILE */
