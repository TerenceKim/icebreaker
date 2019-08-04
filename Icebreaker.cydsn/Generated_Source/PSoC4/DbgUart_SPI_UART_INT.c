/***************************************************************************//**
* \file DbgUart_SPI_UART_INT.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
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

#include "DbgUart_PVT.h"
#include "DbgUart_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (DbgUart_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: DbgUart_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(DbgUart_SPI_UART_ISR)
{
#if (DbgUart_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (DbgUart_INTERNAL_RX_SW_BUFFER_CONST) */

#if (DbgUart_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (DbgUart_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef DbgUart_SPI_UART_ISR_ENTRY_CALLBACK
    DbgUart_SPI_UART_ISR_EntryCallback();
#endif /* DbgUart_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != DbgUart_customIntrHandler)
    {
        DbgUart_customIntrHandler();
    }

    #if(DbgUart_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        DbgUart_ClearSpiExtClkInterruptSource(DbgUart_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (DbgUart_CHECK_RX_SW_BUFFER)
    {
        if (DbgUart_CHECK_INTR_RX_MASKED(DbgUart_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (DbgUart_rxBufferHead + 1u);

                /* Adjust local head index */
                if (DbgUart_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == DbgUart_rxBufferTail)
                {
                    #if (DbgUart_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        DbgUart_INTR_RX_MASK_REG &= ~DbgUart_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) DbgUart_RX_FIFO_RD_REG;
                        DbgUart_rxBufferOverflow = (uint8) DbgUart_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    DbgUart_PutWordInRxBuffer(locHead, DbgUart_RX_FIFO_RD_REG);

                    /* Move head index */
                    DbgUart_rxBufferHead = locHead;
                }
            }
            while(0u != DbgUart_GET_RX_FIFO_ENTRIES);

            DbgUart_ClearRxInterruptSource(DbgUart_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (DbgUart_CHECK_TX_SW_BUFFER)
    {
        if (DbgUart_CHECK_INTR_TX_MASKED(DbgUart_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (DbgUart_txBufferHead != DbgUart_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (DbgUart_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (DbgUart_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    DbgUart_TX_FIFO_WR_REG = DbgUart_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    DbgUart_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    DbgUart_DISABLE_INTR_TX(DbgUart_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (DbgUart_SPI_UART_FIFO_SIZE != DbgUart_GET_TX_FIFO_ENTRIES);

            DbgUart_ClearTxInterruptSource(DbgUart_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef DbgUart_SPI_UART_ISR_EXIT_CALLBACK
    DbgUart_SPI_UART_ISR_ExitCallback();
#endif /* DbgUart_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (DbgUart_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
