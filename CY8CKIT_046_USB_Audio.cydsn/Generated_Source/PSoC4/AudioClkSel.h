/*******************************************************************************
* File Name: AudioClkSel.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_AudioClkSel_H) /* CY_CONTROL_REG_AudioClkSel_H */
#define CY_CONTROL_REG_AudioClkSel_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} AudioClkSel_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    AudioClkSel_Write(uint8 control) ;
uint8   AudioClkSel_Read(void) ;

void AudioClkSel_SaveConfig(void) ;
void AudioClkSel_RestoreConfig(void) ;
void AudioClkSel_Sleep(void) ; 
void AudioClkSel_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define AudioClkSel_Control        (* (reg8 *) AudioClkSel_Sync_ctrl_reg__CONTROL_REG )
#define AudioClkSel_Control_PTR    (  (reg8 *) AudioClkSel_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_AudioClkSel_H */


/* [] END OF FILE */
