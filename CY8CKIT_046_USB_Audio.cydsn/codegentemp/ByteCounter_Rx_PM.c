/*******************************************************************************
* File Name: ByteCounter_Rx_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ByteCounter_Rx.h"

static ByteCounter_Rx_BACKUP_STRUCT ByteCounter_Rx_backup;


/*******************************************************************************
* Function Name: ByteCounter_Rx_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: ByteCounter_Rx_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_Sleep(void)
{
    if(0u != (ByteCounter_Rx_BLOCK_CONTROL_REG & ByteCounter_Rx_MASK))
    {
        ByteCounter_Rx_backup.enableState = 1u;
    }
    else
    {
        ByteCounter_Rx_backup.enableState = 0u;
    }

    ByteCounter_Rx_Stop();
    ByteCounter_Rx_SaveConfig();
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: ByteCounter_Rx_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_Wakeup(void)
{
    ByteCounter_Rx_RestoreConfig();

    if(0u != ByteCounter_Rx_backup.enableState)
    {
        ByteCounter_Rx_Enable();
    }
}


/* [] END OF FILE */
