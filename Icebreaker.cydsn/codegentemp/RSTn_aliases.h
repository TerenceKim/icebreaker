/*******************************************************************************
* File Name: RSTn.h  
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

#if !defined(CY_PINS_RSTn_ALIASES_H) /* Pins RSTn_ALIASES_H */
#define CY_PINS_RSTn_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RSTn_0			(RSTn__0__PC)
#define RSTn_0_PS		(RSTn__0__PS)
#define RSTn_0_PC		(RSTn__0__PC)
#define RSTn_0_DR		(RSTn__0__DR)
#define RSTn_0_SHIFT	(RSTn__0__SHIFT)
#define RSTn_0_INTR	((uint16)((uint16)0x0003u << (RSTn__0__SHIFT*2u)))

#define RSTn_INTR_ALL	 ((uint16)(RSTn_0_INTR))


#endif /* End Pins RSTn_ALIASES_H */


/* [] END OF FILE */
