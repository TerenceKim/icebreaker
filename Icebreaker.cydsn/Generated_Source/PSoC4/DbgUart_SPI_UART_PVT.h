/***************************************************************************//**
* \file DbgUart_SPI_UART_PVT.h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
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

#if !defined(CY_SCB_SPI_UART_PVT_DbgUart_H)
#define CY_SCB_SPI_UART_PVT_DbgUart_H

#include "DbgUart_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (DbgUart_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  DbgUart_rxBufferHead;
    extern volatile uint32  DbgUart_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   DbgUart_rxBufferOverflow;
    /** @} globals */
#endif /* (DbgUart_INTERNAL_RX_SW_BUFFER_CONST) */

#if (DbgUart_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  DbgUart_txBufferHead;
    extern volatile uint32  DbgUart_txBufferTail;
#endif /* (DbgUart_INTERNAL_TX_SW_BUFFER_CONST) */

#if (DbgUart_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 DbgUart_rxBufferInternal[DbgUart_INTERNAL_RX_BUFFER_SIZE];
#endif /* (DbgUart_INTERNAL_RX_SW_BUFFER) */

#if (DbgUart_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 DbgUart_txBufferInternal[DbgUart_TX_BUFFER_SIZE];
#endif /* (DbgUart_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void DbgUart_SpiPostEnable(void);
void DbgUart_SpiStop(void);

#if (DbgUart_SCB_MODE_SPI_CONST_CFG)
    void DbgUart_SpiInit(void);
#endif /* (DbgUart_SCB_MODE_SPI_CONST_CFG) */

#if (DbgUart_SPI_WAKE_ENABLE_CONST)
    void DbgUart_SpiSaveConfig(void);
    void DbgUart_SpiRestoreConfig(void);
#endif /* (DbgUart_SPI_WAKE_ENABLE_CONST) */

void DbgUart_UartPostEnable(void);
void DbgUart_UartStop(void);

#if (DbgUart_SCB_MODE_UART_CONST_CFG)
    void DbgUart_UartInit(void);
#endif /* (DbgUart_SCB_MODE_UART_CONST_CFG) */

#if (DbgUart_UART_WAKE_ENABLE_CONST)
    void DbgUart_UartSaveConfig(void);
    void DbgUart_UartRestoreConfig(void);
#endif /* (DbgUart_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in DbgUart_SetPins() */
#define DbgUart_UART_RX_PIN_ENABLE    (DbgUart_UART_RX)
#define DbgUart_UART_TX_PIN_ENABLE    (DbgUart_UART_TX)

/* UART RTS and CTS position to be used in  DbgUart_SetPins() */
#define DbgUart_UART_RTS_PIN_ENABLE    (0x10u)
#define DbgUart_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define DbgUart_SpiUartEnableIntRx(intSourceMask)  DbgUart_SetRxInterruptMode(intSourceMask)
#define DbgUart_SpiUartEnableIntTx(intSourceMask)  DbgUart_SetTxInterruptMode(intSourceMask)
uint32  DbgUart_SpiUartDisableIntRx(void);
uint32  DbgUart_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_DbgUart_H) */


/* [] END OF FILE */
