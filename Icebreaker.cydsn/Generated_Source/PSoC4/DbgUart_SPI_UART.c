/***************************************************************************//**
* \file DbgUart_SPI_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DbgUart_PVT.h"
#include "DbgUart_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(DbgUart_INTERNAL_RX_SW_BUFFER_CONST)
    /* Start index to put data into the software receive buffer.*/
    volatile uint32 DbgUart_rxBufferHead;
    /* Start index to get data from the software receive buffer.*/
    volatile uint32 DbgUart_rxBufferTail;
    /**
    * \addtogroup group_globals
    * \{
    */
    /** Sets when internal software receive buffer overflow
    *  was occurred.
    */
    volatile uint8  DbgUart_rxBufferOverflow;
    /** \} globals */
#endif /* (DbgUart_INTERNAL_RX_SW_BUFFER_CONST) */

#if(DbgUart_INTERNAL_TX_SW_BUFFER_CONST)
    /* Start index to put data into the software transmit buffer.*/
    volatile uint32 DbgUart_txBufferHead;
    /* Start index to get data from the software transmit buffer.*/
    volatile uint32 DbgUart_txBufferTail;
#endif /* (DbgUart_INTERNAL_TX_SW_BUFFER_CONST) */

#if(DbgUart_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 DbgUart_rxBufferInternal[DbgUart_INTERNAL_RX_BUFFER_SIZE];
#endif /* (DbgUart_INTERNAL_RX_SW_BUFFER) */

#if(DbgUart_INTERNAL_TX_SW_BUFFER)
    volatile uint8 DbgUart_txBufferInternal[DbgUart_TX_BUFFER_SIZE];
#endif /* (DbgUart_INTERNAL_TX_SW_BUFFER) */


#if(DbgUart_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: DbgUart_SpiUartReadRxData
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * \return
    *  Next data element from the receive buffer. 
    *  The amount of data bits to be received depends on RX data bits selection 
    *  (the data bit counting starts from LSB of return value).
    *
    * \globalvars
    *  DbgUart_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  DbgUart_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 DbgUart_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

    #if (DbgUart_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (DbgUart_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (DbgUart_CHECK_RX_SW_BUFFER)
        {
            if (DbgUart_rxBufferHead != DbgUart_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (DbgUart_rxBufferTail + 1u);

                if (DbgUart_INTERNAL_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = DbgUart_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                DbgUart_rxBufferTail = locTail;

                #if (DbgUart_CHECK_UART_RTS_CONTROL_FLOW)
                {
                    /* Check if RX Not Empty is disabled in the interrupt */
                    if (0u == (DbgUart_INTR_RX_MASK_REG & DbgUart_INTR_RX_NOT_EMPTY))
                    {
                        /* Enable RX Not Empty interrupt source to continue
                        * receiving data into software buffer.
                        */
                        DbgUart_INTR_RX_MASK_REG |= DbgUart_INTR_RX_NOT_EMPTY;
                    }
                }
                #endif

            }
        }
        #else
        {
            /* Read data from RX FIFO */
            rxData = DbgUart_RX_FIFO_RD_REG;
        }
        #endif

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: DbgUart_SpiUartGetRxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * \return
    *  Number of received data elements.
    *
    * \globalvars
    *  DbgUart_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  DbgUart_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 DbgUart_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
    #if (DbgUart_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (DbgUart_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (DbgUart_CHECK_RX_SW_BUFFER)
        {
            locHead = DbgUart_rxBufferHead;

            if(locHead >= DbgUart_rxBufferTail)
            {
                size = (locHead - DbgUart_rxBufferTail);
            }
            else
            {
                size = (locHead + (DbgUart_INTERNAL_RX_BUFFER_SIZE - DbgUart_rxBufferTail));
            }
        }
        #else
        {
            size = DbgUart_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: DbgUart_SpiUartClearRxBuffer
    ****************************************************************************//**
    *
    *  Clears the receive buffer and RX FIFO.
    *
    * \globalvars
    *  DbgUart_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  DbgUart_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    void DbgUart_SpiUartClearRxBuffer(void)
    {
        #if (DbgUart_CHECK_RX_SW_BUFFER)
        {
            /* Lock from component interruption */
            DbgUart_DisableInt();

            /* Flush RX software buffer */
            DbgUart_rxBufferHead = DbgUart_rxBufferTail;
            DbgUart_rxBufferOverflow = 0u;

            DbgUart_CLEAR_RX_FIFO;
            DbgUart_ClearRxInterruptSource(DbgUart_INTR_RX_ALL);

            #if (DbgUart_CHECK_UART_RTS_CONTROL_FLOW)
            {
                /* Enable RX Not Empty interrupt source to continue receiving
                * data into software buffer.
                */
                DbgUart_INTR_RX_MASK_REG |= DbgUart_INTR_RX_NOT_EMPTY;
            }
            #endif
            
            /* Release lock */
            DbgUart_EnableInt();
        }
        #else
        {
            DbgUart_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (DbgUart_RX_DIRECTION) */


#if(DbgUart_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: DbgUart_SpiUartWriteTxData
    ****************************************************************************//**
    *
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *   The amount of data bits to be transmitted depends on TX data bits selection 
    *   (the data bit counting starts from LSB of txDataByte).
    *
    * \globalvars
    *  DbgUart_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  DbgUart_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void DbgUart_SpiUartWriteTxData(uint32 txData)
    {
    #if (DbgUart_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (DbgUart_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (DbgUart_CHECK_TX_SW_BUFFER)
        {
            /* Put data directly into the TX FIFO */
            if ((DbgUart_txBufferHead == DbgUart_txBufferTail) &&
                (DbgUart_SPI_UART_FIFO_SIZE != DbgUart_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                DbgUart_TX_FIFO_WR_REG = txData;
            }
            /* Put data into TX software buffer */
            else
            {
                /* Head index to put data */
                locHead = (DbgUart_txBufferHead + 1u);

                /* Adjust TX software buffer index */
                if (DbgUart_TX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                /* Wait for space in TX software buffer */
                while (locHead == DbgUart_txBufferTail)
                {
                }

                /* TX software buffer has at least one room */

                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                DbgUart_ClearTxInterruptSource(DbgUart_INTR_TX_NOT_FULL);

                DbgUart_PutWordInTxBuffer(locHead, txData);

                DbgUart_txBufferHead = locHead;

                /* Check if TX Not Full is disabled in interrupt */
                if (0u == (DbgUart_INTR_TX_MASK_REG & DbgUart_INTR_TX_NOT_FULL))
                {
                    /* Enable TX Not Full interrupt source to transmit from software buffer */
                    DbgUart_INTR_TX_MASK_REG |= (uint32) DbgUart_INTR_TX_NOT_FULL;
                }
            }
        }
        #else
        {
            /* Wait until TX FIFO has space to put data element */
            while (DbgUart_SPI_UART_FIFO_SIZE == DbgUart_GET_TX_FIFO_ENTRIES)
            {
            }

            DbgUart_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: DbgUart_SpiUartPutArray
    ****************************************************************************//**
    *
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * \param wrBuf: pointer to an array of data to be placed in transmit buffer. 
    *  The width of the data to be transmitted depends on TX data width selection 
    *  (the data bit counting starts from LSB for each array element).
    * \param count: number of data elements to be placed in the transmit buffer.
    *
    * \globalvars
    *  DbgUart_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  DbgUart_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void DbgUart_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for (i=0u; i < count; i++)
        {
            DbgUart_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: DbgUart_SpiUartGetTxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of elements currently in the transmit buffer.
    *   - TX software buffer is disabled: returns the number of used entries in
    *     TX FIFO.
    *   - TX software buffer is enabled: returns the number of elements currently
    *     used in the transmit buffer. This number does not include used entries in
    *     the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *     not full.
    *
    * \return
    *  Number of data elements ready to transmit.
    *
    * \globalvars
    *  DbgUart_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  DbgUart_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    uint32 DbgUart_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
    #if (DbgUart_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (DbgUart_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (DbgUart_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = DbgUart_txBufferTail;

            if (DbgUart_txBufferHead >= locTail)
            {
                size = (DbgUart_txBufferHead - locTail);
            }
            else
            {
                size = (DbgUart_txBufferHead + (DbgUart_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = DbgUart_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: DbgUart_SpiUartClearTxBuffer
    ****************************************************************************//**
    *
    *  Clears the transmit buffer and TX FIFO.
    *
    * \globalvars
    *  DbgUart_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  DbgUart_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void DbgUart_SpiUartClearTxBuffer(void)
    {
        #if (DbgUart_CHECK_TX_SW_BUFFER)
        {
            /* Lock from component interruption */
            DbgUart_DisableInt();

            /* Flush TX software buffer */
            DbgUart_txBufferHead = DbgUart_txBufferTail;

            DbgUart_INTR_TX_MASK_REG &= (uint32) ~DbgUart_INTR_TX_NOT_FULL;
            DbgUart_CLEAR_TX_FIFO;
            DbgUart_ClearTxInterruptSource(DbgUart_INTR_TX_ALL);

            /* Release lock */
            DbgUart_EnableInt();
        }
        #else
        {
            DbgUart_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (DbgUart_TX_DIRECTION) */


/*******************************************************************************
* Function Name: DbgUart_SpiUartDisableIntRx
****************************************************************************//**
*
*  Disables the RX interrupt sources.
*
*  \return
*   Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 DbgUart_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = DbgUart_GetRxInterruptMode();

    DbgUart_SetRxInterruptMode(DbgUart_NO_INTR_SOURCES);

    return (intSource);
}


/*******************************************************************************
* Function Name: DbgUart_SpiUartDisableIntTx
****************************************************************************//**
*
*  Disables TX interrupt sources.
*
*  \return
*   Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 DbgUart_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = DbgUart_GetTxInterruptMode();

    DbgUart_SetTxInterruptMode(DbgUart_NO_INTR_SOURCES);

    return (intSourceMask);
}


#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: DbgUart_PutWordInRxBuffer
    ****************************************************************************//**
    *
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param index:      index to store data byte/word in the RX buffer.
    *  \param rxDataByte: byte/word to store.
    *
    *******************************************************************************/
    void DbgUart_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if (DbgUart_ONE_BYTE_WIDTH == DbgUart_rxDataBits)
        {
            DbgUart_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            DbgUart_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            DbgUart_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: DbgUart_GetWordFromRxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \return
    *   Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 DbgUart_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if (DbgUart_ONE_BYTE_WIDTH == DbgUart_rxDataBits)
        {
            value = DbgUart_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) DbgUart_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)DbgUart_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }


    /*******************************************************************************
    * Function Name: DbgUart_PutWordInTxBuffer
    ****************************************************************************//**
    *
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx:        index to store data byte/word in the TX buffer.
    *  \param txDataByte: byte/word to store.
    *
    *******************************************************************************/
    void DbgUart_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if (DbgUart_ONE_BYTE_WIDTH == DbgUart_txDataBits)
        {
            DbgUart_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            DbgUart_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            DbgUart_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: DbgUart_GetWordFromTxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx: index to get data byte/word from the TX buffer.
    *
    *  \return
    *   Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 DbgUart_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if (DbgUart_ONE_BYTE_WIDTH == DbgUart_txDataBits)
        {
            value = (uint32) DbgUart_txBuffer[idx];
        }
        else
        {
            value  = (uint32) DbgUart_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) DbgUart_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }

#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
