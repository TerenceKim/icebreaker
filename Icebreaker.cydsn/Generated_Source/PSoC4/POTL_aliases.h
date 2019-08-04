/*******************************************************************************
* File Name: POTL.h  
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

#if !defined(CY_PINS_POTL_ALIASES_H) /* Pins POTL_ALIASES_H */
#define CY_PINS_POTL_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define POTL_0			(POTL__0__PC)
#define POTL_0_PS		(POTL__0__PS)
#define POTL_0_PC		(POTL__0__PC)
#define POTL_0_DR		(POTL__0__DR)
#define POTL_0_SHIFT	(POTL__0__SHIFT)
#define POTL_0_INTR	((uint16)((uint16)0x0003u << (POTL__0__SHIFT*2u)))

#define POTL_INTR_ALL	 ((uint16)(POTL_0_INTR))


#endif /* End Pins POTL_ALIASES_H */


/* [] END OF FILE */
