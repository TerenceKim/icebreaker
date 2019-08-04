/*******************************************************************************
* File Name: Async_Feedback_Counter_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Async_Feedback_Counter.h"

static Async_Feedback_Counter_backupStruct Async_Feedback_Counter_backup;


/*******************************************************************************
* Function Name: Async_Feedback_Counter_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Async_Feedback_Counter_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Async_Feedback_Counter_SaveConfig(void) 
{
    #if (!Async_Feedback_Counter_UsingFixedFunction)
        Async_Feedback_Counter_backup.TimerUdb = Async_Feedback_Counter_ReadCounter();
        Async_Feedback_Counter_backup.InterruptMaskValue = Async_Feedback_Counter_STATUS_MASK;
        #if (Async_Feedback_Counter_UsingHWCaptureCounter)
            Async_Feedback_Counter_backup.TimerCaptureCounter = Async_Feedback_Counter_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED)
            Async_Feedback_Counter_backup.TimerControlRegister = Async_Feedback_Counter_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Async_Feedback_Counter_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Async_Feedback_Counter_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Async_Feedback_Counter_RestoreConfig(void) 
{   
    #if (!Async_Feedback_Counter_UsingFixedFunction)

        Async_Feedback_Counter_WriteCounter(Async_Feedback_Counter_backup.TimerUdb);
        Async_Feedback_Counter_STATUS_MASK =Async_Feedback_Counter_backup.InterruptMaskValue;
        #if (Async_Feedback_Counter_UsingHWCaptureCounter)
            Async_Feedback_Counter_SetCaptureCount(Async_Feedback_Counter_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED)
            Async_Feedback_Counter_WriteControlRegister(Async_Feedback_Counter_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Async_Feedback_Counter_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Async_Feedback_Counter_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Async_Feedback_Counter_Sleep(void) 
{
    #if(!Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Async_Feedback_Counter_CTRL_ENABLE == (Async_Feedback_Counter_CONTROL & Async_Feedback_Counter_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Async_Feedback_Counter_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Async_Feedback_Counter_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Async_Feedback_Counter_Stop();
    Async_Feedback_Counter_SaveConfig();
}


/*******************************************************************************
* Function Name: Async_Feedback_Counter_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Async_Feedback_Counter_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Async_Feedback_Counter_Wakeup(void) 
{
    Async_Feedback_Counter_RestoreConfig();
    #if(!Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED)
        if(Async_Feedback_Counter_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Async_Feedback_Counter_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
