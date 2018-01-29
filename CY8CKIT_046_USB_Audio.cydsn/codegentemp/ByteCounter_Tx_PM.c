/*******************************************************************************
* File Name: ByteCounter_Tx_PM.c
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

#include "ByteCounter_Tx.h"

static ByteCounter_Tx_BACKUP_STRUCT ByteCounter_Tx_backup;


/*******************************************************************************
* Function Name: ByteCounter_Tx_SaveConfig
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
void ByteCounter_Tx_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: ByteCounter_Tx_Sleep
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
void ByteCounter_Tx_Sleep(void)
{
    if(0u != (ByteCounter_Tx_BLOCK_CONTROL_REG & ByteCounter_Tx_MASK))
    {
        ByteCounter_Tx_backup.enableState = 1u;
    }
    else
    {
        ByteCounter_Tx_backup.enableState = 0u;
    }

    ByteCounter_Tx_Stop();
    ByteCounter_Tx_SaveConfig();
}


/*******************************************************************************
* Function Name: ByteCounter_Tx_RestoreConfig
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
void ByteCounter_Tx_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: ByteCounter_Tx_Wakeup
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
void ByteCounter_Tx_Wakeup(void)
{
    ByteCounter_Tx_RestoreConfig();

    if(0u != ByteCounter_Tx_backup.enableState)
    {
        ByteCounter_Tx_Enable();
    }
}


/* [] END OF FILE */
