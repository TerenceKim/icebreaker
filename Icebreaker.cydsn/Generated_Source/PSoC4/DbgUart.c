/***************************************************************************//**
* \file DbgUart.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (DbgUart_SCB_MODE_I2C_INC)
    #include "DbgUart_I2C_PVT.h"
#endif /* (DbgUart_SCB_MODE_I2C_INC) */

#if (DbgUart_SCB_MODE_EZI2C_INC)
    #include "DbgUart_EZI2C_PVT.h"
#endif /* (DbgUart_SCB_MODE_EZI2C_INC) */

#if (DbgUart_SCB_MODE_SPI_INC || DbgUart_SCB_MODE_UART_INC)
    #include "DbgUart_SPI_UART_PVT.h"
#endif /* (DbgUart_SCB_MODE_SPI_INC || DbgUart_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 DbgUart_scbMode = DbgUart_SCB_MODE_UNCONFIG;
    uint8 DbgUart_scbEnableWake;
    uint8 DbgUart_scbEnableIntr;

    /* I2C configuration variables */
    uint8 DbgUart_mode;
    uint8 DbgUart_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * DbgUart_rxBuffer;
    uint8  DbgUart_rxDataBits;
    uint32 DbgUart_rxBufferSize;

    volatile uint8 * DbgUart_txBuffer;
    uint8  DbgUart_txDataBits;
    uint32 DbgUart_txBufferSize;

    /* EZI2C configuration variables */
    uint8 DbgUart_numberOfAddr;
    uint8 DbgUart_subAddrSize;
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** DbgUart_initVar indicates whether the DbgUart 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the DbgUart_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  DbgUart_Init() function can be called before the 
*  DbgUart_Start() or DbgUart_Enable() function.
*/
uint8 DbgUart_initVar = 0u;


#if (! (DbgUart_SCB_MODE_I2C_CONST_CFG || \
        DbgUart_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * DbgUart_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent DbgUart_Enable() call.
    */
    uint16 DbgUart_IntrTxMask = 0u;
#endif /* (! (DbgUart_SCB_MODE_I2C_CONST_CFG || \
              DbgUart_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (DbgUart_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_DbgUart_CUSTOM_INTR_HANDLER)
    void (*DbgUart_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_DbgUart_CUSTOM_INTR_HANDLER) */
#endif /* (DbgUart_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void DbgUart_ScbEnableIntr(void);
static void DbgUart_ScbModeStop(void);
static void DbgUart_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: DbgUart_Init
****************************************************************************//**
*
*  Initializes the DbgUart component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  DbgUart_I2CInit, DbgUart_SpiInit, 
*  DbgUart_UartInit or DbgUart_EzI2CInit.
*
*******************************************************************************/
void DbgUart_Init(void)
{
#if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    if (DbgUart_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        DbgUart_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (DbgUart_SCB_MODE_I2C_CONST_CFG)
    DbgUart_I2CInit();

#elif (DbgUart_SCB_MODE_SPI_CONST_CFG)
    DbgUart_SpiInit();

#elif (DbgUart_SCB_MODE_UART_CONST_CFG)
    DbgUart_UartInit();

#elif (DbgUart_SCB_MODE_EZI2C_CONST_CFG)
    DbgUart_EzI2CInit();

#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: DbgUart_Enable
****************************************************************************//**
*
*  Enables DbgUart component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  DbgUart_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The DbgUart configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured DbgUart”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void DbgUart_Enable(void)
{
#if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!DbgUart_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        DbgUart_CTRL_REG |= DbgUart_CTRL_ENABLED;

        DbgUart_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        DbgUart_ScbModePostEnable();
    }
#else
    DbgUart_CTRL_REG |= DbgUart_CTRL_ENABLED;

    DbgUart_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    DbgUart_ScbModePostEnable();
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: DbgUart_Start
****************************************************************************//**
*
*  Invokes DbgUart_Init() and DbgUart_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  DbgUart_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void DbgUart_Start(void)
{
    if (0u == DbgUart_initVar)
    {
        DbgUart_Init();
        DbgUart_initVar = 1u; /* Component was initialized */
    }

    DbgUart_Enable();
}


/*******************************************************************************
* Function Name: DbgUart_Stop
****************************************************************************//**
*
*  Disables the DbgUart component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function DbgUart_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void DbgUart_Stop(void)
{
#if (DbgUart_SCB_IRQ_INTERNAL)
    DbgUart_DisableInt();
#endif /* (DbgUart_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    DbgUart_ScbModeStop();

    /* Disable SCB IP */
    DbgUart_CTRL_REG &= (uint32) ~DbgUart_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    DbgUart_SetTxInterruptMode(DbgUart_NO_INTR_SOURCES);

#if (DbgUart_SCB_IRQ_INTERNAL)
    DbgUart_ClearPendingInt();
#endif /* (DbgUart_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: DbgUart_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void DbgUart_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = DbgUart_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~DbgUart_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (DbgUart_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    DbgUart_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: DbgUart_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void DbgUart_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = DbgUart_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~DbgUart_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (DbgUart_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    DbgUart_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (DbgUart_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: DbgUart_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void DbgUart_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_DbgUart_CUSTOM_INTR_HANDLER)
        DbgUart_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_DbgUart_CUSTOM_INTR_HANDLER) */
    }
#endif /* (DbgUart_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: DbgUart_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void DbgUart_ScbEnableIntr(void)
{
#if (DbgUart_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != DbgUart_scbEnableIntr)
        {
            DbgUart_EnableInt();
        }

    #else
        DbgUart_EnableInt();

    #endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (DbgUart_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: DbgUart_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void DbgUart_ScbModePostEnable(void)
{
#if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!DbgUart_CY_SCBIP_V1)
    if (DbgUart_SCB_MODE_SPI_RUNTM_CFG)
    {
        DbgUart_SpiPostEnable();
    }
    else if (DbgUart_SCB_MODE_UART_RUNTM_CFG)
    {
        DbgUart_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!DbgUart_CY_SCBIP_V1) */

#elif (DbgUart_SCB_MODE_SPI_CONST_CFG)
    DbgUart_SpiPostEnable();

#elif (DbgUart_SCB_MODE_UART_CONST_CFG)
    DbgUart_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: DbgUart_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void DbgUart_ScbModeStop(void)
{
#if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    if (DbgUart_SCB_MODE_I2C_RUNTM_CFG)
    {
        DbgUart_I2CStop();
    }
    else if (DbgUart_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        DbgUart_EzI2CStop();
    }
#if (!DbgUart_CY_SCBIP_V1)
    else if (DbgUart_SCB_MODE_SPI_RUNTM_CFG)
    {
        DbgUart_SpiStop();
    }
    else if (DbgUart_SCB_MODE_UART_RUNTM_CFG)
    {
        DbgUart_UartStop();
    }
#endif /* (!DbgUart_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (DbgUart_SCB_MODE_I2C_CONST_CFG)
    DbgUart_I2CStop();

#elif (DbgUart_SCB_MODE_EZI2C_CONST_CFG)
    DbgUart_EzI2CStop();

#elif (DbgUart_SCB_MODE_SPI_CONST_CFG)
    DbgUart_SpiStop();

#elif (DbgUart_SCB_MODE_UART_CONST_CFG)
    DbgUart_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: DbgUart_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void DbgUart_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[DbgUart_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!DbgUart_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!DbgUart_CY_SCBIP_V1) */
        
        uint32 hsiomSel[DbgUart_SCB_PINS_NUMBER] = 
        {
            DbgUart_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            DbgUart_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (DbgUart_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (DbgUart_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < DbgUart_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = DbgUart_PIN_DM_ALG_HIZ;
        }

        if ((DbgUart_SCB_MODE_I2C   == mode) ||
            (DbgUart_SCB_MODE_EZI2C == mode))
        {
        #if (DbgUart_RX_SCL_MOSI_PIN)
            hsiomSel[DbgUart_RX_SCL_MOSI_PIN_INDEX] = DbgUart_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [DbgUart_RX_SCL_MOSI_PIN_INDEX] = DbgUart_PIN_DM_OD_LO;
        #elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX] = DbgUart_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX] = DbgUart_PIN_DM_OD_LO;
        #else
        #endif /* (DbgUart_RX_SCL_MOSI_PIN) */
        
        #if (DbgUart_TX_SDA_MISO_PIN)
            hsiomSel[DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_PIN_DM_OD_LO;
        #endif /* (DbgUart_TX_SDA_MISO_PIN) */
        }
    #if (!DbgUart_CY_SCBIP_V1)
        else if (DbgUart_SCB_MODE_SPI == mode)
        {
        #if (DbgUart_RX_SCL_MOSI_PIN)
            hsiomSel[DbgUart_RX_SCL_MOSI_PIN_INDEX] = DbgUart_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX] = DbgUart_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (DbgUart_RX_SCL_MOSI_PIN) */
        
        #if (DbgUart_TX_SDA_MISO_PIN)
            hsiomSel[DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (DbgUart_TX_SDA_MISO_PIN) */
        
        #if (DbgUart_CTS_SCLK_PIN)
            hsiomSel[DbgUart_CTS_SCLK_PIN_INDEX] = DbgUart_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (DbgUart_CTS_SCLK_PIN) */

            if (DbgUart_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[DbgUart_RX_SCL_MOSI_PIN_INDEX] = DbgUart_PIN_DM_DIG_HIZ;
                pinsDm[DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                pinsDm[DbgUart_CTS_SCLK_PIN_INDEX] = DbgUart_PIN_DM_DIG_HIZ;

            #if (DbgUart_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[DbgUart_RTS_SS0_PIN_INDEX] = DbgUart_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [DbgUart_RTS_SS0_PIN_INDEX] = DbgUart_PIN_DM_DIG_HIZ;
            #endif /* (DbgUart_RTS_SS0_PIN) */

            #if (DbgUart_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= DbgUart_TX_SDA_MISO_PIN_MASK;
            #endif /* (DbgUart_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[DbgUart_RX_SCL_MOSI_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                pinsDm[DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_PIN_DM_DIG_HIZ;
                pinsDm[DbgUart_CTS_SCLK_PIN_INDEX] = DbgUart_PIN_DM_STRONG;

            #if (DbgUart_RTS_SS0_PIN)
                hsiomSel [DbgUart_RTS_SS0_PIN_INDEX] = DbgUart_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [DbgUart_RTS_SS0_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                pinsInBuf |= DbgUart_RTS_SS0_PIN_MASK;
            #endif /* (DbgUart_RTS_SS0_PIN) */

            #if (DbgUart_SS1_PIN)
                hsiomSel [DbgUart_SS1_PIN_INDEX] = DbgUart_SS1_HSIOM_SEL_SPI;
                pinsDm   [DbgUart_SS1_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                pinsInBuf |= DbgUart_SS1_PIN_MASK;
            #endif /* (DbgUart_SS1_PIN) */

            #if (DbgUart_SS2_PIN)
                hsiomSel [DbgUart_SS2_PIN_INDEX] = DbgUart_SS2_HSIOM_SEL_SPI;
                pinsDm   [DbgUart_SS2_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                pinsInBuf |= DbgUart_SS2_PIN_MASK;
            #endif /* (DbgUart_SS2_PIN) */

            #if (DbgUart_SS3_PIN)
                hsiomSel [DbgUart_SS3_PIN_INDEX] = DbgUart_SS3_HSIOM_SEL_SPI;
                pinsDm   [DbgUart_SS3_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                pinsInBuf |= DbgUart_SS3_PIN_MASK;
            #endif /* (DbgUart_SS3_PIN) */

                /* Disable input buffers */
            #if (DbgUart_RX_SCL_MOSI_PIN)
                pinsInBuf |= DbgUart_RX_SCL_MOSI_PIN_MASK;
            #elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= DbgUart_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (DbgUart_RX_SCL_MOSI_PIN) */

            #if (DbgUart_CTS_SCLK_PIN)
                pinsInBuf |= DbgUart_CTS_SCLK_PIN_MASK;
            #endif /* (DbgUart_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (DbgUart_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (DbgUart_TX_SDA_MISO_PIN)
                hsiomSel[DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_PIN_DM_OD_LO;
            #endif /* (DbgUart_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (DbgUart_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (DbgUart_RX_SCL_MOSI_PIN)
                    hsiomSel[DbgUart_RX_SCL_MOSI_PIN_INDEX] = DbgUart_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [DbgUart_RX_SCL_MOSI_PIN_INDEX] = DbgUart_PIN_DM_DIG_HIZ;
                #elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX] = DbgUart_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX] = DbgUart_PIN_DM_DIG_HIZ;
                #else
                #endif /* (DbgUart_RX_SCL_MOSI_PIN) */
                }

                if (0u != (DbgUart_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (DbgUart_TX_SDA_MISO_PIN)
                    hsiomSel[DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [DbgUart_TX_SDA_MISO_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= DbgUart_TX_SDA_MISO_PIN_MASK;
                #endif /* (DbgUart_TX_SDA_MISO_PIN) */
                }

            #if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
                if (DbgUart_UART_MODE_STD == subMode)
                {
                    if (0u != (DbgUart_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (DbgUart_CTS_SCLK_PIN)
                        hsiomSel[DbgUart_CTS_SCLK_PIN_INDEX] = DbgUart_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [DbgUart_CTS_SCLK_PIN_INDEX] = DbgUart_PIN_DM_DIG_HIZ;
                    #endif /* (DbgUart_CTS_SCLK_PIN) */
                    }

                    if (0u != (DbgUart_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (DbgUart_RTS_SS0_PIN)
                        hsiomSel[DbgUart_RTS_SS0_PIN_INDEX] = DbgUart_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [DbgUart_RTS_SS0_PIN_INDEX] = DbgUart_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= DbgUart_RTS_SS0_PIN_MASK;
                    #endif /* (DbgUart_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */
            }
        }
    #endif /* (!DbgUart_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (DbgUart_RX_SCL_MOSI_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_RX_SCL_MOSI_HSIOM_REG,
                                       DbgUart_RX_SCL_MOSI_HSIOM_MASK,
                                       DbgUart_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[DbgUart_RX_SCL_MOSI_PIN_INDEX]);

        DbgUart_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[DbgUart_RX_SCL_MOSI_PIN_INDEX]);

        #if (!DbgUart_CY_SCBIP_V1)
            DbgUart_SET_INP_DIS(DbgUart_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         DbgUart_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & DbgUart_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!DbgUart_CY_SCBIP_V1) */
    
    #elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       DbgUart_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       DbgUart_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        DbgUart_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        DbgUart_SET_INP_DIS(DbgUart_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     DbgUart_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & DbgUart_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        DbgUart_SET_INCFG_TYPE(DbgUart_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        DbgUart_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        DbgUart_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        DbgUart_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (DbgUart_RX_WAKE_SCL_MOSI_PIN) */

    #if (DbgUart_TX_SDA_MISO_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_TX_SDA_MISO_HSIOM_REG,
                                       DbgUart_TX_SDA_MISO_HSIOM_MASK,
                                       DbgUart_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[DbgUart_TX_SDA_MISO_PIN_INDEX]);

        DbgUart_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[DbgUart_TX_SDA_MISO_PIN_INDEX]);

    #if (!DbgUart_CY_SCBIP_V1)
        DbgUart_SET_INP_DIS(DbgUart_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     DbgUart_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & DbgUart_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!DbgUart_CY_SCBIP_V1) */
    #endif /* (DbgUart_RX_SCL_MOSI_PIN) */

    #if (DbgUart_CTS_SCLK_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_CTS_SCLK_HSIOM_REG,
                                       DbgUart_CTS_SCLK_HSIOM_MASK,
                                       DbgUart_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[DbgUart_CTS_SCLK_PIN_INDEX]);

        DbgUart_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[DbgUart_CTS_SCLK_PIN_INDEX]);

        DbgUart_SET_INP_DIS(DbgUart_uart_cts_spi_sclk_INP_DIS,
                                     DbgUart_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & DbgUart_CTS_SCLK_PIN_MASK)));
    #endif /* (DbgUart_CTS_SCLK_PIN) */

    #if (DbgUart_RTS_SS0_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_RTS_SS0_HSIOM_REG,
                                       DbgUart_RTS_SS0_HSIOM_MASK,
                                       DbgUart_RTS_SS0_HSIOM_POS,
                                       hsiomSel[DbgUart_RTS_SS0_PIN_INDEX]);

        DbgUart_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[DbgUart_RTS_SS0_PIN_INDEX]);

        DbgUart_SET_INP_DIS(DbgUart_uart_rts_spi_ss0_INP_DIS,
                                     DbgUart_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & DbgUart_RTS_SS0_PIN_MASK)));
    #endif /* (DbgUart_RTS_SS0_PIN) */

    #if (DbgUart_SS1_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_SS1_HSIOM_REG,
                                       DbgUart_SS1_HSIOM_MASK,
                                       DbgUart_SS1_HSIOM_POS,
                                       hsiomSel[DbgUart_SS1_PIN_INDEX]);

        DbgUart_spi_ss1_SetDriveMode((uint8) pinsDm[DbgUart_SS1_PIN_INDEX]);

        DbgUart_SET_INP_DIS(DbgUart_spi_ss1_INP_DIS,
                                     DbgUart_spi_ss1_MASK,
                                     (0u != (pinsInBuf & DbgUart_SS1_PIN_MASK)));
    #endif /* (DbgUart_SS1_PIN) */

    #if (DbgUart_SS2_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_SS2_HSIOM_REG,
                                       DbgUart_SS2_HSIOM_MASK,
                                       DbgUart_SS2_HSIOM_POS,
                                       hsiomSel[DbgUart_SS2_PIN_INDEX]);

        DbgUart_spi_ss2_SetDriveMode((uint8) pinsDm[DbgUart_SS2_PIN_INDEX]);

        DbgUart_SET_INP_DIS(DbgUart_spi_ss2_INP_DIS,
                                     DbgUart_spi_ss2_MASK,
                                     (0u != (pinsInBuf & DbgUart_SS2_PIN_MASK)));
    #endif /* (DbgUart_SS2_PIN) */

    #if (DbgUart_SS3_PIN)
        DbgUart_SET_HSIOM_SEL(DbgUart_SS3_HSIOM_REG,
                                       DbgUart_SS3_HSIOM_MASK,
                                       DbgUart_SS3_HSIOM_POS,
                                       hsiomSel[DbgUart_SS3_PIN_INDEX]);

        DbgUart_spi_ss3_SetDriveMode((uint8) pinsDm[DbgUart_SS3_PIN_INDEX]);

        DbgUart_SET_INP_DIS(DbgUart_spi_ss3_INP_DIS,
                                     DbgUart_spi_ss3_MASK,
                                     (0u != (pinsInBuf & DbgUart_SS3_PIN_MASK)));
    #endif /* (DbgUart_SS3_PIN) */
    }

#endif /* (DbgUart_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: DbgUart_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void DbgUart_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (DbgUart_CTRL_REG & DbgUart_CTRL_EC_AM_MODE)) &&
            (0u == (DbgUart_I2C_CTRL_REG & DbgUart_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            DbgUart_CTRL_REG &= ~DbgUart_CTRL_EC_AM_MODE;
            DbgUart_CTRL_REG |=  DbgUart_CTRL_EC_AM_MODE;
        }

        DbgUart_I2C_SLAVE_CMD_REG = DbgUart_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */


/* [] END OF FILE */
