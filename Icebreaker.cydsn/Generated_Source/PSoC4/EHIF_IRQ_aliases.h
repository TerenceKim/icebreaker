/*******************************************************************************
* File Name: EHIF_IRQ.h  
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

#if !defined(CY_PINS_EHIF_IRQ_ALIASES_H) /* Pins EHIF_IRQ_ALIASES_H */
#define CY_PINS_EHIF_IRQ_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define EHIF_IRQ_0			(EHIF_IRQ__0__PC)
#define EHIF_IRQ_0_PS		(EHIF_IRQ__0__PS)
#define EHIF_IRQ_0_PC		(EHIF_IRQ__0__PC)
#define EHIF_IRQ_0_DR		(EHIF_IRQ__0__DR)
#define EHIF_IRQ_0_SHIFT	(EHIF_IRQ__0__SHIFT)
#define EHIF_IRQ_0_INTR	((uint16)((uint16)0x0003u << (EHIF_IRQ__0__SHIFT*2u)))

#define EHIF_IRQ_INTR_ALL	 ((uint16)(EHIF_IRQ_0_INTR))


#endif /* End Pins EHIF_IRQ_ALIASES_H */


/* [] END OF FILE */
