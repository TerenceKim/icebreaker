/*******************************************************************************
* File Name: CodecI2CM_PM.c
* Version 3.10
*
* Description:
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CodecI2CM.h"
#include "CodecI2CM_PVT.h"

#if(CodecI2CM_SCB_MODE_I2C_INC)
    #include "CodecI2CM_I2C_PVT.h"
#endif /* (CodecI2CM_SCB_MODE_I2C_INC) */

#if(CodecI2CM_SCB_MODE_EZI2C_INC)
    #include "CodecI2CM_EZI2C_PVT.h"
#endif /* (CodecI2CM_SCB_MODE_EZI2C_INC) */

#if(CodecI2CM_SCB_MODE_SPI_INC || CodecI2CM_SCB_MODE_UART_INC)
    #include "CodecI2CM_SPI_UART_PVT.h"
#endif /* (CodecI2CM_SCB_MODE_SPI_INC || CodecI2CM_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(CodecI2CM_SCB_MODE_UNCONFIG_CONST_CFG || \
   (CodecI2CM_SCB_MODE_I2C_CONST_CFG   && (!CodecI2CM_I2C_WAKE_ENABLE_CONST))   || \
   (CodecI2CM_SCB_MODE_EZI2C_CONST_CFG && (!CodecI2CM_EZI2C_WAKE_ENABLE_CONST)) || \
   (CodecI2CM_SCB_MODE_SPI_CONST_CFG   && (!CodecI2CM_SPI_WAKE_ENABLE_CONST))   || \
   (CodecI2CM_SCB_MODE_UART_CONST_CFG  && (!CodecI2CM_UART_WAKE_ENABLE_CONST)))

    CodecI2CM_BACKUP_STRUCT CodecI2CM_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: CodecI2CM_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component to enter Deep Sleep.
*  The "Enable wakeup from Sleep Mode" selection has an influence on
*  this function implementation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CodecI2CM_Sleep(void)
{
#if(CodecI2CM_SCB_MODE_UNCONFIG_CONST_CFG)

    if(CodecI2CM_SCB_WAKE_ENABLE_CHECK)
    {
        if(CodecI2CM_SCB_MODE_I2C_RUNTM_CFG)
        {
            CodecI2CM_I2CSaveConfig();
        }
        else if(CodecI2CM_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            CodecI2CM_EzI2CSaveConfig();
        }
    #if(!CodecI2CM_CY_SCBIP_V1)
        else if(CodecI2CM_SCB_MODE_SPI_RUNTM_CFG)
        {
            CodecI2CM_SpiSaveConfig();
        }
        else if(CodecI2CM_SCB_MODE_UART_RUNTM_CFG)
        {
            CodecI2CM_UartSaveConfig();
        }
    #endif /* (!CodecI2CM_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        CodecI2CM_backup.enableState = (uint8) CodecI2CM_GET_CTRL_ENABLED;

        if(0u != CodecI2CM_backup.enableState)
        {
            CodecI2CM_Stop();
        }
    }

#else

    #if (CodecI2CM_SCB_MODE_I2C_CONST_CFG && CodecI2CM_I2C_WAKE_ENABLE_CONST)
        CodecI2CM_I2CSaveConfig();

    #elif (CodecI2CM_SCB_MODE_EZI2C_CONST_CFG && CodecI2CM_EZI2C_WAKE_ENABLE_CONST)
        CodecI2CM_EzI2CSaveConfig();

    #elif (CodecI2CM_SCB_MODE_SPI_CONST_CFG && CodecI2CM_SPI_WAKE_ENABLE_CONST)
        CodecI2CM_SpiSaveConfig();

    #elif (CodecI2CM_SCB_MODE_UART_CONST_CFG && CodecI2CM_UART_WAKE_ENABLE_CONST)
        CodecI2CM_UartSaveConfig();

    #else

        CodecI2CM_backup.enableState = (uint8) CodecI2CM_GET_CTRL_ENABLED;

        if(0u != CodecI2CM_backup.enableState)
        {
            CodecI2CM_Stop();
        }

    #endif /* defined (CodecI2CM_SCB_MODE_I2C_CONST_CFG) && (CodecI2CM_I2C_WAKE_ENABLE_CONST) */

#endif /* (CodecI2CM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: CodecI2CM_Wakeup
********************************************************************************
*
* Summary:
*  Prepares the component for the Active mode operation after exiting
*  Deep Sleep. The "Enable wakeup from Sleep Mode" option has an influence
*  on this function implementation.
*  This function should not be called after exiting Sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CodecI2CM_Wakeup(void)
{
#if(CodecI2CM_SCB_MODE_UNCONFIG_CONST_CFG)

    if(CodecI2CM_SCB_WAKE_ENABLE_CHECK)
    {
        if(CodecI2CM_SCB_MODE_I2C_RUNTM_CFG)
        {
            CodecI2CM_I2CRestoreConfig();
        }
        else if(CodecI2CM_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            CodecI2CM_EzI2CRestoreConfig();
        }
    #if(!CodecI2CM_CY_SCBIP_V1)
        else if(CodecI2CM_SCB_MODE_SPI_RUNTM_CFG)
        {
            CodecI2CM_SpiRestoreConfig();
        }
        else if(CodecI2CM_SCB_MODE_UART_RUNTM_CFG)
        {
            CodecI2CM_UartRestoreConfig();
        }
    #endif /* (!CodecI2CM_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != CodecI2CM_backup.enableState)
        {
            CodecI2CM_Enable();
        }
    }

#else

    #if (CodecI2CM_SCB_MODE_I2C_CONST_CFG  && CodecI2CM_I2C_WAKE_ENABLE_CONST)
        CodecI2CM_I2CRestoreConfig();

    #elif (CodecI2CM_SCB_MODE_EZI2C_CONST_CFG && CodecI2CM_EZI2C_WAKE_ENABLE_CONST)
        CodecI2CM_EzI2CRestoreConfig();

    #elif (CodecI2CM_SCB_MODE_SPI_CONST_CFG && CodecI2CM_SPI_WAKE_ENABLE_CONST)
        CodecI2CM_SpiRestoreConfig();

    #elif (CodecI2CM_SCB_MODE_UART_CONST_CFG && CodecI2CM_UART_WAKE_ENABLE_CONST)
        CodecI2CM_UartRestoreConfig();

    #else

        if(0u != CodecI2CM_backup.enableState)
        {
            CodecI2CM_Enable();
        }

    #endif /* (CodecI2CM_I2C_WAKE_ENABLE_CONST) */

#endif /* (CodecI2CM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
