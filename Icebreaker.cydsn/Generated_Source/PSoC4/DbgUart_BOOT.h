/***************************************************************************//**
* \file DbgUart_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_DbgUart_H)
#define CY_SCB_BOOT_DbgUart_H

#include "DbgUart_PVT.h"

#if (DbgUart_SCB_MODE_I2C_INC)
    #include "DbgUart_I2C.h"
#endif /* (DbgUart_SCB_MODE_I2C_INC) */

#if (DbgUart_SCB_MODE_EZI2C_INC)
    #include "DbgUart_EZI2C.h"
#endif /* (DbgUart_SCB_MODE_EZI2C_INC) */

#if (DbgUart_SCB_MODE_SPI_INC || DbgUart_SCB_MODE_UART_INC)
    #include "DbgUart_SPI_UART.h"
#endif /* (DbgUart_SCB_MODE_SPI_INC || DbgUart_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define DbgUart_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_DbgUart) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (DbgUart_SCB_MODE_I2C_INC)
    #define DbgUart_I2C_BTLDR_COMM_ENABLED     (DbgUart_BTLDR_COMM_ENABLED && \
                                                            (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             DbgUart_I2C_SLAVE_CONST))
#else
     #define DbgUart_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (DbgUart_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (DbgUart_SCB_MODE_EZI2C_INC)
    #define DbgUart_EZI2C_BTLDR_COMM_ENABLED   (DbgUart_BTLDR_COMM_ENABLED && \
                                                         DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define DbgUart_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (DbgUart_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (DbgUart_SCB_MODE_SPI_INC)
    #define DbgUart_SPI_BTLDR_COMM_ENABLED     (DbgUart_BTLDR_COMM_ENABLED && \
                                                            (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             DbgUart_SPI_SLAVE_CONST))
#else
        #define DbgUart_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (DbgUart_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (DbgUart_SCB_MODE_UART_INC)
       #define DbgUart_UART_BTLDR_COMM_ENABLED    (DbgUart_BTLDR_COMM_ENABLED && \
                                                            (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (DbgUart_UART_RX_DIRECTION && \
                                                              DbgUart_UART_TX_DIRECTION)))
#else
     #define DbgUart_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (DbgUart_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define DbgUart_BTLDR_COMM_MODE_ENABLED    (DbgUart_I2C_BTLDR_COMM_ENABLED   || \
                                                     DbgUart_SPI_BTLDR_COMM_ENABLED   || \
                                                     DbgUart_EZI2C_BTLDR_COMM_ENABLED || \
                                                     DbgUart_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void DbgUart_I2CCyBtldrCommStart(void);
    void DbgUart_I2CCyBtldrCommStop (void);
    void DbgUart_I2CCyBtldrCommReset(void);
    cystatus DbgUart_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus DbgUart_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (DbgUart_SCB_MODE_I2C_CONST_CFG)
        #define DbgUart_CyBtldrCommStart   DbgUart_I2CCyBtldrCommStart
        #define DbgUart_CyBtldrCommStop    DbgUart_I2CCyBtldrCommStop
        #define DbgUart_CyBtldrCommReset   DbgUart_I2CCyBtldrCommReset
        #define DbgUart_CyBtldrCommRead    DbgUart_I2CCyBtldrCommRead
        #define DbgUart_CyBtldrCommWrite   DbgUart_I2CCyBtldrCommWrite
    #endif /* (DbgUart_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void DbgUart_EzI2CCyBtldrCommStart(void);
    void DbgUart_EzI2CCyBtldrCommStop (void);
    void DbgUart_EzI2CCyBtldrCommReset(void);
    cystatus DbgUart_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus DbgUart_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (DbgUart_SCB_MODE_EZI2C_CONST_CFG)
        #define DbgUart_CyBtldrCommStart   DbgUart_EzI2CCyBtldrCommStart
        #define DbgUart_CyBtldrCommStop    DbgUart_EzI2CCyBtldrCommStop
        #define DbgUart_CyBtldrCommReset   DbgUart_EzI2CCyBtldrCommReset
        #define DbgUart_CyBtldrCommRead    DbgUart_EzI2CCyBtldrCommRead
        #define DbgUart_CyBtldrCommWrite   DbgUart_EzI2CCyBtldrCommWrite
    #endif /* (DbgUart_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void DbgUart_SpiCyBtldrCommStart(void);
    void DbgUart_SpiCyBtldrCommStop (void);
    void DbgUart_SpiCyBtldrCommReset(void);
    cystatus DbgUart_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus DbgUart_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (DbgUart_SCB_MODE_SPI_CONST_CFG)
        #define DbgUart_CyBtldrCommStart   DbgUart_SpiCyBtldrCommStart
        #define DbgUart_CyBtldrCommStop    DbgUart_SpiCyBtldrCommStop
        #define DbgUart_CyBtldrCommReset   DbgUart_SpiCyBtldrCommReset
        #define DbgUart_CyBtldrCommRead    DbgUart_SpiCyBtldrCommRead
        #define DbgUart_CyBtldrCommWrite   DbgUart_SpiCyBtldrCommWrite
    #endif /* (DbgUart_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void DbgUart_UartCyBtldrCommStart(void);
    void DbgUart_UartCyBtldrCommStop (void);
    void DbgUart_UartCyBtldrCommReset(void);
    cystatus DbgUart_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus DbgUart_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (DbgUart_SCB_MODE_UART_CONST_CFG)
        #define DbgUart_CyBtldrCommStart   DbgUart_UartCyBtldrCommStart
        #define DbgUart_CyBtldrCommStop    DbgUart_UartCyBtldrCommStop
        #define DbgUart_CyBtldrCommReset   DbgUart_UartCyBtldrCommReset
        #define DbgUart_CyBtldrCommRead    DbgUart_UartCyBtldrCommRead
        #define DbgUart_CyBtldrCommWrite   DbgUart_UartCyBtldrCommWrite
    #endif /* (DbgUart_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_BTLDR_COMM_ENABLED)
    #if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void DbgUart_CyBtldrCommStart(void);
        void DbgUart_CyBtldrCommStop (void);
        void DbgUart_CyBtldrCommReset(void);
        cystatus DbgUart_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus DbgUart_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_DbgUart)
        #define CyBtldrCommStart    DbgUart_CyBtldrCommStart
        #define CyBtldrCommStop     DbgUart_CyBtldrCommStop
        #define CyBtldrCommReset    DbgUart_CyBtldrCommReset
        #define CyBtldrCommWrite    DbgUart_CyBtldrCommWrite
        #define CyBtldrCommRead     DbgUart_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_DbgUart) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (DbgUart_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define DbgUart_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define DbgUart_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define DbgUart_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define DbgUart_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef DbgUart_SPI_BYTE_TO_BYTE
    #define DbgUart_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef DbgUart_UART_BYTE_TO_BYTE
    #define DbgUart_UART_BYTE_TO_BYTE  (175u)
#endif /* DbgUart_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_DbgUart_H) */


/* [] END OF FILE */
