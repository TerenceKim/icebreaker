/***************************************************************************//**
* \file DbgUart_PM.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DbgUart.h"
#include "DbgUart_PVT.h"

#if(DbgUart_SCB_MODE_I2C_INC)
    #include "DbgUart_I2C_PVT.h"
#endif /* (DbgUart_SCB_MODE_I2C_INC) */

#if(DbgUart_SCB_MODE_EZI2C_INC)
    #include "DbgUart_EZI2C_PVT.h"
#endif /* (DbgUart_SCB_MODE_EZI2C_INC) */

#if(DbgUart_SCB_MODE_SPI_INC || DbgUart_SCB_MODE_UART_INC)
    #include "DbgUart_SPI_UART_PVT.h"
#endif /* (DbgUart_SCB_MODE_SPI_INC || DbgUart_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG || \
   (DbgUart_SCB_MODE_I2C_CONST_CFG   && (!DbgUart_I2C_WAKE_ENABLE_CONST))   || \
   (DbgUart_SCB_MODE_EZI2C_CONST_CFG && (!DbgUart_EZI2C_WAKE_ENABLE_CONST)) || \
   (DbgUart_SCB_MODE_SPI_CONST_CFG   && (!DbgUart_SPI_WAKE_ENABLE_CONST))   || \
   (DbgUart_SCB_MODE_UART_CONST_CFG  && (!DbgUart_UART_WAKE_ENABLE_CONST)))

    DbgUart_BACKUP_STRUCT DbgUart_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: DbgUart_Sleep
****************************************************************************//**
*
*  Prepares the DbgUart component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the DbgUart_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void DbgUart_Sleep(void)
{
#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)

    if(DbgUart_SCB_WAKE_ENABLE_CHECK)
    {
        if(DbgUart_SCB_MODE_I2C_RUNTM_CFG)
        {
            DbgUart_I2CSaveConfig();
        }
        else if(DbgUart_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            DbgUart_EzI2CSaveConfig();
        }
    #if(!DbgUart_CY_SCBIP_V1)
        else if(DbgUart_SCB_MODE_SPI_RUNTM_CFG)
        {
            DbgUart_SpiSaveConfig();
        }
        else if(DbgUart_SCB_MODE_UART_RUNTM_CFG)
        {
            DbgUart_UartSaveConfig();
        }
    #endif /* (!DbgUart_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        DbgUart_backup.enableState = (uint8) DbgUart_GET_CTRL_ENABLED;

        if(0u != DbgUart_backup.enableState)
        {
            DbgUart_Stop();
        }
    }

#else

    #if (DbgUart_SCB_MODE_I2C_CONST_CFG && DbgUart_I2C_WAKE_ENABLE_CONST)
        DbgUart_I2CSaveConfig();

    #elif (DbgUart_SCB_MODE_EZI2C_CONST_CFG && DbgUart_EZI2C_WAKE_ENABLE_CONST)
        DbgUart_EzI2CSaveConfig();

    #elif (DbgUart_SCB_MODE_SPI_CONST_CFG && DbgUart_SPI_WAKE_ENABLE_CONST)
        DbgUart_SpiSaveConfig();

    #elif (DbgUart_SCB_MODE_UART_CONST_CFG && DbgUart_UART_WAKE_ENABLE_CONST)
        DbgUart_UartSaveConfig();

    #else

        DbgUart_backup.enableState = (uint8) DbgUart_GET_CTRL_ENABLED;

        if(0u != DbgUart_backup.enableState)
        {
            DbgUart_Stop();
        }

    #endif /* defined (DbgUart_SCB_MODE_I2C_CONST_CFG) && (DbgUart_I2C_WAKE_ENABLE_CONST) */

#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: DbgUart_Wakeup
****************************************************************************//**
*
*  Prepares the DbgUart component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the DbgUart_Wakeup() function without first calling the 
*   DbgUart_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void DbgUart_Wakeup(void)
{
#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)

    if(DbgUart_SCB_WAKE_ENABLE_CHECK)
    {
        if(DbgUart_SCB_MODE_I2C_RUNTM_CFG)
        {
            DbgUart_I2CRestoreConfig();
        }
        else if(DbgUart_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            DbgUart_EzI2CRestoreConfig();
        }
    #if(!DbgUart_CY_SCBIP_V1)
        else if(DbgUart_SCB_MODE_SPI_RUNTM_CFG)
        {
            DbgUart_SpiRestoreConfig();
        }
        else if(DbgUart_SCB_MODE_UART_RUNTM_CFG)
        {
            DbgUart_UartRestoreConfig();
        }
    #endif /* (!DbgUart_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != DbgUart_backup.enableState)
        {
            DbgUart_Enable();
        }
    }

#else

    #if (DbgUart_SCB_MODE_I2C_CONST_CFG  && DbgUart_I2C_WAKE_ENABLE_CONST)
        DbgUart_I2CRestoreConfig();

    #elif (DbgUart_SCB_MODE_EZI2C_CONST_CFG && DbgUart_EZI2C_WAKE_ENABLE_CONST)
        DbgUart_EzI2CRestoreConfig();

    #elif (DbgUart_SCB_MODE_SPI_CONST_CFG && DbgUart_SPI_WAKE_ENABLE_CONST)
        DbgUart_SpiRestoreConfig();

    #elif (DbgUart_SCB_MODE_UART_CONST_CFG && DbgUart_UART_WAKE_ENABLE_CONST)
        DbgUart_UartRestoreConfig();

    #else

        if(0u != DbgUart_backup.enableState)
        {
            DbgUart_Enable();
        }

    #endif /* (DbgUart_I2C_WAKE_ENABLE_CONST) */

#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
