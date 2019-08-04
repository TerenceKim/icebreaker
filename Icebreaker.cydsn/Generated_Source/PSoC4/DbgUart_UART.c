/***************************************************************************//**
* \file DbgUart_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
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
#include "cyapicallbacks.h"

#if (DbgUart_UART_WAKE_ENABLE_CONST && DbgUart_UART_RX_WAKEUP_IRQ)
    /**
    * \addtogroup group_globals
    * \{
    */
    /** This global variable determines whether to enable Skip Start
    * functionality when DbgUart_Sleep() function is called:
    * 0 – disable, other values – enable. Default value is 1.
    * It is only available when Enable wakeup from Deep Sleep Mode is enabled.
    */
    uint8 DbgUart_skipStart = 1u;
    /** \} globals */
#endif /* (DbgUart_UART_WAKE_ENABLE_CONST && DbgUart_UART_RX_WAKEUP_IRQ) */

#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const DbgUart_UART_INIT_STRUCT DbgUart_configUart =
    {
        DbgUart_UART_SUB_MODE,
        DbgUart_UART_DIRECTION,
        DbgUart_UART_DATA_BITS_NUM,
        DbgUart_UART_PARITY_TYPE,
        DbgUart_UART_STOP_BITS_NUM,
        DbgUart_UART_OVS_FACTOR,
        DbgUart_UART_IRDA_LOW_POWER,
        DbgUart_UART_MEDIAN_FILTER_ENABLE,
        DbgUart_UART_RETRY_ON_NACK,
        DbgUart_UART_IRDA_POLARITY,
        DbgUart_UART_DROP_ON_PARITY_ERR,
        DbgUart_UART_DROP_ON_FRAME_ERR,
        DbgUart_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        DbgUart_UART_MP_MODE_ENABLE,
        DbgUart_UART_MP_ACCEPT_ADDRESS,
        DbgUart_UART_MP_RX_ADDRESS,
        DbgUart_UART_MP_RX_ADDRESS_MASK,
        (uint32) DbgUart_SCB_IRQ_INTERNAL,
        DbgUart_UART_INTR_RX_MASK,
        DbgUart_UART_RX_TRIGGER_LEVEL,
        DbgUart_UART_INTR_TX_MASK,
        DbgUart_UART_TX_TRIGGER_LEVEL,
        (uint8) DbgUart_UART_BYTE_MODE_ENABLE,
        (uint8) DbgUart_UART_CTS_ENABLE,
        (uint8) DbgUart_UART_CTS_POLARITY,
        (uint8) DbgUart_UART_RTS_POLARITY,
        (uint8) DbgUart_UART_RTS_FIFO_LEVEL,
        (uint8) DbgUart_UART_RX_BREAK_WIDTH
    };


    /*******************************************************************************
    * Function Name: DbgUart_UartInit
    ****************************************************************************//**
    *
    *  Configures the DbgUart for UART operation.
    *
    *  This function is intended specifically to be used when the DbgUart
    *  configuration is set to “Unconfigured DbgUart” in the customizer.
    *  After initializing the DbgUart in UART mode using this function,
    *  the component can be enabled using the DbgUart_Start() or
    * DbgUart_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration
    *  settings. This structure contains the same information that would otherwise
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of
    *   fields. These fields match the selections available in the customizer.
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void DbgUart_UartInit(const DbgUart_UART_INIT_STRUCT *config)
    {
        uint32 pinsConfig;

        if (NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Get direction to configure UART pins: TX, RX or TX+RX */
            pinsConfig  = config->direction;

        #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (DbgUart_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (DbgUart_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */

            /* Configure pins */
            DbgUart_SetPins(DbgUart_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            DbgUart_scbMode       = (uint8) DbgUart_SCB_MODE_UART;
            DbgUart_scbEnableWake = (uint8) config->enableWake;
            DbgUart_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            DbgUart_rxBuffer      =         config->rxBuffer;
            DbgUart_rxDataBits    = (uint8) config->dataBits;
            DbgUart_rxBufferSize  =         config->rxBufferSize;

            /* Set TX direction internal variables */
            DbgUart_txBuffer      =         config->txBuffer;
            DbgUart_txDataBits    = (uint8) config->dataBits;
            DbgUart_txBufferSize  =         config->txBufferSize;

            /* Configure UART interface */
            if(DbgUart_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                DbgUart_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (DbgUart_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (DbgUart_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                DbgUart_CTRL_REG  = DbgUart_GET_CTRL_OVS(config->oversample);
            }

            DbgUart_CTRL_REG     |= DbgUart_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             DbgUart_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             DbgUart_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            DbgUart_UART_CTRL_REG = DbgUart_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            DbgUart_UART_RX_CTRL_REG = DbgUart_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        DbgUart_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        DbgUart_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        DbgUart_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        DbgUart_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr)   |
                                        DbgUart_GET_UART_RX_CTRL_BREAK_WIDTH(config->breakWidth);

            if(DbgUart_UART_PARITY_NONE != config->parity)
            {
               DbgUart_UART_RX_CTRL_REG |= DbgUart_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    DbgUart_UART_RX_CTRL_PARITY_ENABLED;
            }

            DbgUart_RX_CTRL_REG      = DbgUart_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                DbgUart_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                DbgUart_GET_UART_RX_CTRL_ENABLED(config->direction);

            DbgUart_RX_FIFO_CTRL_REG = DbgUart_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            DbgUart_RX_MATCH_REG     = DbgUart_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                DbgUart_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            DbgUart_UART_TX_CTRL_REG = DbgUart_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                DbgUart_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(DbgUart_UART_PARITY_NONE != config->parity)
            {
               DbgUart_UART_TX_CTRL_REG |= DbgUart_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    DbgUart_UART_TX_CTRL_PARITY_ENABLED;
            }

            DbgUart_TX_CTRL_REG      = DbgUart_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                DbgUart_GET_UART_TX_CTRL_ENABLED(config->direction);

            DbgUart_TX_FIFO_CTRL_REG = DbgUart_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
            DbgUart_UART_FLOW_CTRL_REG = DbgUart_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            DbgUart_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            DbgUart_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            DbgUart_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (DbgUart_ISR_NUMBER);
            CyIntSetPriority(DbgUart_ISR_NUMBER, DbgUart_ISR_PRIORITY);
            (void) CyIntSetVector(DbgUart_ISR_NUMBER, &DbgUart_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(DbgUart_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (DbgUart_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(DbgUart_RX_WAKE_ISR_NUMBER, DbgUart_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(DbgUart_RX_WAKE_ISR_NUMBER, &DbgUart_UART_WAKEUP_ISR);
        #endif /* (DbgUart_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            DbgUart_INTR_I2C_EC_MASK_REG = DbgUart_NO_INTR_SOURCES;
            DbgUart_INTR_SPI_EC_MASK_REG = DbgUart_NO_INTR_SOURCES;
            DbgUart_INTR_SLAVE_MASK_REG  = DbgUart_NO_INTR_SOURCES;
            DbgUart_INTR_MASTER_MASK_REG = DbgUart_NO_INTR_SOURCES;
            DbgUart_INTR_RX_MASK_REG     = config->rxInterruptMask;
            DbgUart_INTR_TX_MASK_REG     = config->txInterruptMask;

            /* Configure TX interrupt sources to restore. */
            DbgUart_IntrTxMask = LO16(DbgUart_INTR_TX_MASK_REG);

            /* Clear RX buffer indexes */
            DbgUart_rxBufferHead     = 0u;
            DbgUart_rxBufferTail     = 0u;
            DbgUart_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            DbgUart_txBufferHead = 0u;
            DbgUart_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: DbgUart_UartInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the UART operation.
    *
    *******************************************************************************/
    void DbgUart_UartInit(void)
    {
        /* Configure UART interface */
        DbgUart_CTRL_REG = DbgUart_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        DbgUart_UART_CTRL_REG = DbgUart_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        DbgUart_UART_RX_CTRL_REG = DbgUart_UART_DEFAULT_UART_RX_CTRL;
        DbgUart_RX_CTRL_REG      = DbgUart_UART_DEFAULT_RX_CTRL;
        DbgUart_RX_FIFO_CTRL_REG = DbgUart_UART_DEFAULT_RX_FIFO_CTRL;
        DbgUart_RX_MATCH_REG     = DbgUart_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        DbgUart_UART_TX_CTRL_REG = DbgUart_UART_DEFAULT_UART_TX_CTRL;
        DbgUart_TX_CTRL_REG      = DbgUart_UART_DEFAULT_TX_CTRL;
        DbgUart_TX_FIFO_CTRL_REG = DbgUart_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
        DbgUart_UART_FLOW_CTRL_REG = DbgUart_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(DbgUart_SCB_IRQ_INTERNAL)
        CyIntDisable    (DbgUart_ISR_NUMBER);
        CyIntSetPriority(DbgUart_ISR_NUMBER, DbgUart_ISR_PRIORITY);
        (void) CyIntSetVector(DbgUart_ISR_NUMBER, &DbgUart_SPI_UART_ISR);
    #endif /* (DbgUart_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(DbgUart_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (DbgUart_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(DbgUart_RX_WAKE_ISR_NUMBER, DbgUart_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(DbgUart_RX_WAKE_ISR_NUMBER, &DbgUart_UART_WAKEUP_ISR);
    #endif /* (DbgUart_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        DbgUart_INTR_I2C_EC_MASK_REG = DbgUart_UART_DEFAULT_INTR_I2C_EC_MASK;
        DbgUart_INTR_SPI_EC_MASK_REG = DbgUart_UART_DEFAULT_INTR_SPI_EC_MASK;
        DbgUart_INTR_SLAVE_MASK_REG  = DbgUart_UART_DEFAULT_INTR_SLAVE_MASK;
        DbgUart_INTR_MASTER_MASK_REG = DbgUart_UART_DEFAULT_INTR_MASTER_MASK;
        DbgUart_INTR_RX_MASK_REG     = DbgUart_UART_DEFAULT_INTR_RX_MASK;
        DbgUart_INTR_TX_MASK_REG     = DbgUart_UART_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        DbgUart_IntrTxMask = LO16(DbgUart_INTR_TX_MASK_REG);

    #if(DbgUart_INTERNAL_RX_SW_BUFFER_CONST)
        DbgUart_rxBufferHead     = 0u;
        DbgUart_rxBufferTail     = 0u;
        DbgUart_rxBufferOverflow = 0u;
    #endif /* (DbgUart_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(DbgUart_INTERNAL_TX_SW_BUFFER_CONST)
        DbgUart_txBufferHead = 0u;
        DbgUart_txBufferTail = 0u;
    #endif /* (DbgUart_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: DbgUart_UartPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be
*  controlled by the SCB UART.
*
*******************************************************************************/
void DbgUart_UartPostEnable(void)
{
#if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (DbgUart_TX_SDA_MISO_PIN)
        if (DbgUart_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            DbgUart_SET_HSIOM_SEL(DbgUart_TX_SDA_MISO_HSIOM_REG, DbgUart_TX_SDA_MISO_HSIOM_MASK,
                                           DbgUart_TX_SDA_MISO_HSIOM_POS, DbgUart_TX_SDA_MISO_HSIOM_SEL_UART);
        }
    #endif /* (DbgUart_TX_SDA_MISO_PIN_PIN) */

    #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
        #if (DbgUart_RTS_SS0_PIN)
            if (DbgUart_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                DbgUart_SET_HSIOM_SEL(DbgUart_RTS_SS0_HSIOM_REG, DbgUart_RTS_SS0_HSIOM_MASK,
                                               DbgUart_RTS_SS0_HSIOM_POS, DbgUart_RTS_SS0_HSIOM_SEL_UART);
            }
        #endif /* (DbgUart_RTS_SS0_PIN) */
    #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */

#else
    #if (DbgUart_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        DbgUart_SET_HSIOM_SEL(DbgUart_TX_HSIOM_REG, DbgUart_TX_HSIOM_MASK,
                                       DbgUart_TX_HSIOM_POS, DbgUart_TX_HSIOM_SEL_UART);
    #endif /* (DbgUart_UART_TX_PIN) */

    #if (DbgUart_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        DbgUart_SET_HSIOM_SEL(DbgUart_RTS_HSIOM_REG, DbgUart_RTS_HSIOM_MASK,
                                       DbgUart_RTS_HSIOM_POS, DbgUart_RTS_HSIOM_SEL_UART);
    #endif /* (DbgUart_UART_RTS_PIN) */
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    DbgUart_SetTxInterruptMode(DbgUart_IntrTxMask);
}


/*******************************************************************************
* Function Name: DbgUart_UartStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature
*  to not cause it to trigger after the component is enabled.
*
*******************************************************************************/
void DbgUart_UartStop(void)
{
#if(DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (DbgUart_TX_SDA_MISO_PIN)
        if (DbgUart_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            DbgUart_SET_HSIOM_SEL(DbgUart_TX_SDA_MISO_HSIOM_REG, DbgUart_TX_SDA_MISO_HSIOM_MASK,
                                           DbgUart_TX_SDA_MISO_HSIOM_POS, DbgUart_TX_SDA_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (DbgUart_TX_SDA_MISO_PIN_PIN) */

    #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
        #if (DbgUart_RTS_SS0_PIN)
            if (DbgUart_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                DbgUart_uart_rts_spi_ss0_Write(DbgUart_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                DbgUart_SET_HSIOM_SEL(DbgUart_RTS_SS0_HSIOM_REG, DbgUart_RTS_SS0_HSIOM_MASK,
                                               DbgUart_RTS_SS0_HSIOM_POS, DbgUart_RTS_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (DbgUart_RTS_SS0_PIN) */
    #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */

#else
    #if (DbgUart_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        DbgUart_SET_HSIOM_SEL(DbgUart_TX_HSIOM_REG, DbgUart_TX_HSIOM_MASK,
                                       DbgUart_TX_HSIOM_POS, DbgUart_TX_HSIOM_SEL_GPIO);
    #endif /* (DbgUart_UART_TX_PIN) */

    #if (DbgUart_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        DbgUart_rts_Write(DbgUart_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        DbgUart_SET_HSIOM_SEL(DbgUart_RTS_HSIOM_REG, DbgUart_RTS_HSIOM_MASK,
                                       DbgUart_RTS_HSIOM_POS, DbgUart_RTS_HSIOM_SEL_GPIO);
    #endif /* (DbgUart_UART_RTS_PIN) */

#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (DbgUart_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    DbgUart_UART_RX_CTRL_REG &= (uint32) ~DbgUart_UART_RX_CTRL_SKIP_START;
#endif /* (DbgUart_UART_WAKE_ENABLE_CONST) */

    /* Store TX interrupt sources (exclude level triggered). */
    DbgUart_IntrTxMask = LO16(DbgUart_GetTxInterruptMode() & DbgUart_INTR_UART_TX_RESTORE);
}


/*******************************************************************************
* Function Name: DbgUart_UartSetRxAddress
****************************************************************************//**
*
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
*  \param address: Address for hardware address detection.
*
*******************************************************************************/
void DbgUart_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = DbgUart_RX_MATCH_REG;

    matchReg &= ((uint32) ~DbgUart_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & DbgUart_RX_MATCH_ADDR_MASK)); /* Set address  */

    DbgUart_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: DbgUart_UartSetRxAddressMask
****************************************************************************//**
*
*  Sets the hardware address mask for the UART in the Multiprocessor mode.
*
*  \param addressMask: Address mask.
*   - Bit value 0 – excludes bit from address comparison.
*   - Bit value 1 – the bit needs to match with the corresponding bit
*     of the address.
*
*******************************************************************************/
void DbgUart_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = DbgUart_RX_MATCH_REG;

    matchReg &= ((uint32) ~DbgUart_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << DbgUart_RX_MATCH_MASK_POS));

    DbgUart_RX_MATCH_REG = matchReg;
}


#if(DbgUart_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: DbgUart_UartGetChar
    ****************************************************************************//**
    *
    *  Retrieves next data element from receive buffer.
    *  This function is designed for ASCII characters and returns a char where
    *  1 to 255 are valid characters and 0 indicates an error occurred or no data
    *  is present.
    *  - RX software buffer is disabled: Returns data element retrieved from RX
    *    FIFO.
    *  - RX software buffer is enabled: Returns data element from the software
    *    receive buffer.
    *
    *  \return
    *   Next data element from the receive buffer. ASCII character values from
    *   1 to 255 are valid. A returned zero signifies an error condition or no
    *   data available.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check DbgUart_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 DbgUart_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != DbgUart_SpiUartGetRxBufferSize())
        {
            rxData = DbgUart_SpiUartReadRxData();
        }

        if (DbgUart_CHECK_INTR_RX(DbgUart_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            DbgUart_ClearRxInterruptSource(DbgUart_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: DbgUart_UartGetByte
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer, returns the
    *  received byte and error condition.
    *   - The RX software buffer is disabled: returns the data element retrieved
    *     from the RX FIFO. Undefined data will be returned if the RX FIFO is
    *     empty.
    *   - The RX software buffer is enabled: returns data element from the
    *     software receive buffer.
    *
    *  \return
    *   Bits 7-0 contain the next data element from the receive buffer and
    *   other bits contain the error condition.
    *   - DbgUart_UART_RX_OVERFLOW - Attempt to write to a full
    *     receiver FIFO.
    *   - DbgUart_UART_RX_UNDERFLOW    Attempt to read from an empty
    *     receiver FIFO.
    *   - DbgUart_UART_RX_FRAME_ERROR - UART framing error detected.
    *   - DbgUart_UART_RX_PARITY_ERROR - UART parity error detected.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check DbgUart_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 DbgUart_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (DbgUart_CHECK_RX_SW_BUFFER)
        {
            DbgUart_DisableInt();
        }
        #endif

        if (0u != DbgUart_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (DbgUart_CHECK_RX_SW_BUFFER)
            {
                DbgUart_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = DbgUart_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the
            * case of empty. Otherwise the first received byte will be read.
            */
            rxData = DbgUart_RX_FIFO_RD_REG;


            /* Enables interrupt to receive more bytes. */
            #if (DbgUart_CHECK_RX_SW_BUFFER)
            {

                /* The byte has been read from RX FIFO. Clear RX interrupt to
                * not involve interrupt handler when RX FIFO is empty.
                */
                DbgUart_ClearRxInterruptSource(DbgUart_INTR_RX_NOT_EMPTY);

                DbgUart_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (DbgUart_GetRxInterruptSource() & DbgUart_INTR_RX_ERR);
        DbgUart_ClearRxInterruptSource(DbgUart_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: DbgUart_UartSetRtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of RTS output signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of RTS output signal.
        *   - DbgUart_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   - DbgUart_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        *******************************************************************************/
        void DbgUart_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                DbgUart_UART_FLOW_CTRL_REG |= (uint32)  DbgUart_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                DbgUart_UART_FLOW_CTRL_REG &= (uint32) ~DbgUart_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: DbgUart_UartSetRtsFifoLevel
        ****************************************************************************//**
        *
        *  Sets level in the RX FIFO for RTS signal activation.
        *  While the RX FIFO has fewer entries than the RX FIFO level the RTS signal
        *  remains active, otherwise the RTS signal becomes inactive.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param level: Level in the RX FIFO for RTS signal activation.
        *   The range of valid level values is between 0 and RX FIFO depth - 1.
        *   Setting level value to 0 disables RTS signal activation.
        *
        *******************************************************************************/
        void DbgUart_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = DbgUart_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~DbgUart_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (DbgUart_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            DbgUart_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */

#endif /* (DbgUart_UART_RX_DIRECTION) */


#if(DbgUart_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: DbgUart_UartPutString
    ****************************************************************************//**
    *
    *  Places a NULL terminated string in the transmit buffer to be sent at the
    *  next available bus time.
    *  This function is blocking and waits until there is a space available to put
    *  requested data in transmit buffer.
    *
    *  \param string: pointer to the null terminated string array to be placed in the
    *   transmit buffer.
    *
    *******************************************************************************/
    void DbgUart_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            DbgUart_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: DbgUart_UartPutCRLF
    ****************************************************************************//**
    *
    *  Places byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) in the transmit buffer.
    *  This function is blocking and waits until there is a space available to put
    *  all requested data in transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *
    *******************************************************************************/
    void DbgUart_UartPutCRLF(uint32 txDataByte)
    {
        DbgUart_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        DbgUart_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        DbgUart_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: DbgUartSCB_UartEnableCts
        ****************************************************************************//**
        *
        *  Enables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void DbgUart_UartEnableCts(void)
        {
            DbgUart_UART_FLOW_CTRL_REG |= (uint32)  DbgUart_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: DbgUart_UartDisableCts
        ****************************************************************************//**
        *
        *  Disables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void DbgUart_UartDisableCts(void)
        {
            DbgUart_UART_FLOW_CTRL_REG &= (uint32) ~DbgUart_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: DbgUart_UartSetCtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of CTS input signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        * \param
        * polarity: Active polarity of CTS output signal.
        *   - DbgUart_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   - DbgUart_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        *******************************************************************************/
        void DbgUart_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                DbgUart_UART_FLOW_CTRL_REG |= (uint32)  DbgUart_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                DbgUart_UART_FLOW_CTRL_REG &= (uint32) ~DbgUart_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */


    /*******************************************************************************
    * Function Name: DbgUart_UartSendBreakBlocking
    ****************************************************************************//**
    *
    * Sends a break condition (logic low) of specified width on UART TX line.
    * Blocks until break is completed. Only call this function when UART TX FIFO
    * and shifter are empty.
    *
    * \param breakWidth
    * Width of break condition. Valid range is 4 to 16 bits.
    *
    * \note
    * Before sending break all UART TX interrupt sources are disabled. The state
    * of UART TX interrupt sources is restored before function returns.
    *
    * \sideeffect
    * If this function is called while there is data in the TX FIFO or shifter that
    * data will be shifted out in packets the size of breakWidth.
    *
    *******************************************************************************/
    void DbgUart_UartSendBreakBlocking(uint32 breakWidth)
    {
        uint32 txCtrlReg;
        uint32 txIntrReg;

        /* Disable all UART TX interrupt source and clear UART TX Done history */
        txIntrReg = DbgUart_GetTxInterruptMode();
        DbgUart_SetTxInterruptMode(0u);
        DbgUart_ClearTxInterruptSource(DbgUart_INTR_TX_UART_DONE);

        /* Store TX CTRL configuration */
        txCtrlReg = DbgUart_TX_CTRL_REG;

        /* Set break width */
        DbgUart_TX_CTRL_REG = (DbgUart_TX_CTRL_REG & (uint32) ~DbgUart_TX_CTRL_DATA_WIDTH_MASK) |
                                        DbgUart_GET_TX_CTRL_DATA_WIDTH(breakWidth);

        /* Generate break */
        DbgUart_TX_FIFO_WR_REG = 0u;

        /* Wait for break completion */
        while (0u == (DbgUart_GetTxInterruptSource() & DbgUart_INTR_TX_UART_DONE))
        {
        }

        /* Clear all UART TX interrupt sources to  */
        DbgUart_ClearTxInterruptSource(DbgUart_INTR_TX_ALL);

        /* Restore TX interrupt sources and data width */
        DbgUart_TX_CTRL_REG = txCtrlReg;
        DbgUart_SetTxInterruptMode(txIntrReg);
    }
#endif /* (DbgUart_UART_TX_DIRECTION) */


#if (DbgUart_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: DbgUart_UartSaveConfig
    ****************************************************************************//**
    *
    *  Clears and enables an interrupt on a falling edge of the Rx input. The GPIO
    *  interrupt does not track in the active mode, therefore requires to be
    *  cleared by this API.
    *
    *******************************************************************************/
    void DbgUart_UartSaveConfig(void)
    {
    #if (DbgUart_UART_RX_WAKEUP_IRQ)
        /* Set SKIP_START if requested (set by default). */
        if (0u != DbgUart_skipStart)
        {
            DbgUart_UART_RX_CTRL_REG |= (uint32)  DbgUart_UART_RX_CTRL_SKIP_START;
        }
        else
        {
            DbgUart_UART_RX_CTRL_REG &= (uint32) ~DbgUart_UART_RX_CTRL_SKIP_START;
        }

        /* Clear RX GPIO interrupt status and pending interrupt in NVIC because
        * falling edge on RX line occurs while UART communication in active mode.
        * Enable interrupt: next interrupt trigger should wakeup device.
        */
        DbgUart_CLEAR_UART_RX_WAKE_INTR;
        DbgUart_RxWakeClearPendingInt();
        DbgUart_RxWakeEnableInt();
    #endif /* (DbgUart_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: DbgUart_UartRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    *******************************************************************************/
    void DbgUart_UartRestoreConfig(void)
    {
    #if (DbgUart_UART_RX_WAKEUP_IRQ)
        /* Disable interrupt: no more triggers in active mode */
        DbgUart_RxWakeDisableInt();
    #endif /* (DbgUart_UART_RX_WAKEUP_IRQ) */
    }


    #if (DbgUart_UART_RX_WAKEUP_IRQ)
        /*******************************************************************************
        * Function Name: DbgUart_UART_WAKEUP_ISR
        ****************************************************************************//**
        *
        *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
        *  event. This event is configured to trigger on a falling edge of the RX line.
        *
        *******************************************************************************/
        CY_ISR(DbgUart_UART_WAKEUP_ISR)
        {
        #ifdef DbgUart_UART_WAKEUP_ISR_ENTRY_CALLBACK
            DbgUart_UART_WAKEUP_ISR_EntryCallback();
        #endif /* DbgUart_UART_WAKEUP_ISR_ENTRY_CALLBACK */

            DbgUart_CLEAR_UART_RX_WAKE_INTR;

        #ifdef DbgUart_UART_WAKEUP_ISR_EXIT_CALLBACK
            DbgUart_UART_WAKEUP_ISR_ExitCallback();
        #endif /* DbgUart_UART_WAKEUP_ISR_EXIT_CALLBACK */
        }
    #endif /* (DbgUart_UART_RX_WAKEUP_IRQ) */
#endif /* (DbgUart_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
