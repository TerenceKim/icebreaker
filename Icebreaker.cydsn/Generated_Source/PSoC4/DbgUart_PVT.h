/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_DbgUart_H)
#define CY_SCB_PVT_DbgUart_H

#include "DbgUart.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define DbgUart_SetI2CExtClkInterruptMode(interruptMask) DbgUart_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define DbgUart_ClearI2CExtClkInterruptSource(interruptMask) DbgUart_CLEAR_INTR_I2C_EC(interruptMask)
#define DbgUart_GetI2CExtClkInterruptSource()                (DbgUart_INTR_I2C_EC_REG)
#define DbgUart_GetI2CExtClkInterruptMode()                  (DbgUart_INTR_I2C_EC_MASK_REG)
#define DbgUart_GetI2CExtClkInterruptSourceMasked()          (DbgUart_INTR_I2C_EC_MASKED_REG)

#if (!DbgUart_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define DbgUart_SetSpiExtClkInterruptMode(interruptMask) \
                                                                DbgUart_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define DbgUart_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                DbgUart_CLEAR_INTR_SPI_EC(interruptMask)
    #define DbgUart_GetExtSpiClkInterruptSource()                 (DbgUart_INTR_SPI_EC_REG)
    #define DbgUart_GetExtSpiClkInterruptMode()                   (DbgUart_INTR_SPI_EC_MASK_REG)
    #define DbgUart_GetExtSpiClkInterruptSourceMasked()           (DbgUart_INTR_SPI_EC_MASKED_REG)
#endif /* (!DbgUart_CY_SCBIP_V1) */

#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void DbgUart_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (DbgUart_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_DbgUart_CUSTOM_INTR_HANDLER)
    extern cyisraddress DbgUart_customIntrHandler;
#endif /* !defined (CY_REMOVE_DbgUart_CUSTOM_INTR_HANDLER) */
#endif /* (DbgUart_SCB_IRQ_INTERNAL) */

extern DbgUart_BACKUP_STRUCT DbgUart_backup;

#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 DbgUart_scbMode;
    extern uint8 DbgUart_scbEnableWake;
    extern uint8 DbgUart_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 DbgUart_mode;
    extern uint8 DbgUart_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * DbgUart_rxBuffer;
    extern uint8   DbgUart_rxDataBits;
    extern uint32  DbgUart_rxBufferSize;

    extern volatile uint8 * DbgUart_txBuffer;
    extern uint8   DbgUart_txDataBits;
    extern uint32  DbgUart_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 DbgUart_numberOfAddr;
    extern uint8 DbgUart_subAddrSize;
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (DbgUart_SCB_MODE_I2C_CONST_CFG || \
        DbgUart_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 DbgUart_IntrTxMask;
#endif /* (! (DbgUart_SCB_MODE_I2C_CONST_CFG || \
              DbgUart_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define DbgUart_SCB_MODE_I2C_RUNTM_CFG     (DbgUart_SCB_MODE_I2C      == DbgUart_scbMode)
    #define DbgUart_SCB_MODE_SPI_RUNTM_CFG     (DbgUart_SCB_MODE_SPI      == DbgUart_scbMode)
    #define DbgUart_SCB_MODE_UART_RUNTM_CFG    (DbgUart_SCB_MODE_UART     == DbgUart_scbMode)
    #define DbgUart_SCB_MODE_EZI2C_RUNTM_CFG   (DbgUart_SCB_MODE_EZI2C    == DbgUart_scbMode)
    #define DbgUart_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (DbgUart_SCB_MODE_UNCONFIG == DbgUart_scbMode)

    /* Defines wakeup enable */
    #define DbgUart_SCB_WAKE_ENABLE_CHECK       (0u != DbgUart_scbEnableWake)
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!DbgUart_CY_SCBIP_V1)
    #define DbgUart_SCB_PINS_NUMBER    (7u)
#else
    #define DbgUart_SCB_PINS_NUMBER    (2u)
#endif /* (!DbgUart_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_DbgUart_H) */


/* [] END OF FILE */
