/*******************************************************************************
* File Name: CodecI2CM_I2C_MASTER.c
* Version 3.10
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  I2C Master mode.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CodecI2CM_PVT.h"
#include "CodecI2CM_I2C_PVT.h"

#if(CodecI2CM_I2C_MASTER_CONST)

/***************************************
*      I2C Master Private Vars
***************************************/

/* Master variables */
volatile uint16 CodecI2CM_mstrStatus;      /* Master Status byte  */
volatile uint8  CodecI2CM_mstrControl;     /* Master Control byte */

/* Receive buffer variables */
volatile uint8 * CodecI2CM_mstrRdBufPtr;   /* Pointer to Master Read buffer */
volatile uint32  CodecI2CM_mstrRdBufSize;  /* Master Read buffer size       */
volatile uint32  CodecI2CM_mstrRdBufIndex; /* Master Read buffer Index      */

/* Transmit buffer variables */
volatile uint8 * CodecI2CM_mstrWrBufPtr;   /* Pointer to Master Write buffer */
volatile uint32  CodecI2CM_mstrWrBufSize;  /* Master Write buffer size       */
volatile uint32  CodecI2CM_mstrWrBufIndex; /* Master Write buffer Index      */
volatile uint32  CodecI2CM_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */

#if (!CodecI2CM_CY_SCBIP_V0 && \
    CodecI2CM_I2C_MULTI_MASTER_SLAVE_CONST && CodecI2CM_I2C_WAKE_ENABLE_CONST)
    static void CodecI2CM_I2CMasterDisableEcAm(void);
#endif /* (!CodecI2CM_CY_SCBIP_V0) */


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterWriteBuf
********************************************************************************
*
* Summary:
* Automatically writes an entire buffer of data to a slave device.
* Once the data transfer is initiated by this function, further data transfer
* is handled by the included ISR.
* Enables the I2C interrupt and clears SCB_ I2C_MSTAT_WR_CMPLT status.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer of data to be sent.
*  cnt:       Size of buffer to send.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Error status.
*
* Global variables:
*  CodecI2CM_mstrStatus  - used to store current status of I2C Master.
*  CodecI2CM_state       - used to store current state of software FSM.
*  CodecI2CM_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  CodecI2CM_mstrWrBufPtr - used to store pointer to master write buffer.
*  CodecI2CM_mstrWrBufIndex - used to current index within master write
*  buffer.
*  CodecI2CM_mstrWrBufSize - used to store master write buffer size.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterWriteBuf(uint32 slaveAddress, uint8 * wrData, uint32 cnt, uint32 mode)
{
    uint32 errStatus;

    errStatus = CodecI2CM_I2C_MSTR_NOT_READY;

    if(NULL != wrData)  /* Check buffer pointer */
    {
        /* Check FSM state and bus before generating Start/ReStart condition */
        if(CodecI2CM_CHECK_I2C_FSM_IDLE)
        {
            CodecI2CM_DisableInt();  /* Lock from interruption */

            /* Check bus state */
            errStatus = CodecI2CM_CHECK_I2C_STATUS(CodecI2CM_I2C_STATUS_BUS_BUSY) ?
                            CodecI2CM_I2C_MSTR_BUS_BUSY : CodecI2CM_I2C_MSTR_NO_ERROR;
        }
        else if(CodecI2CM_CHECK_I2C_FSM_HALT)
        {
            CodecI2CM_mstrStatus &= (uint16) ~CodecI2CM_I2C_MSTAT_XFER_HALT;
                              errStatus  = CodecI2CM_I2C_MSTR_NO_ERROR;
        }
        else
        {
            /* Unexpected FSM state: exit */
        }
    }

    /* Check if master is ready to start  */
    if(CodecI2CM_I2C_MSTR_NO_ERROR == errStatus) /* No error proceed */
    {
    #if (!CodecI2CM_CY_SCBIP_V0 && \
        CodecI2CM_I2C_MULTI_MASTER_SLAVE_CONST && CodecI2CM_I2C_WAKE_ENABLE_CONST)
            CodecI2CM_I2CMasterDisableEcAm();
    #endif /* (!CodecI2CM_CY_SCBIP_V0) */

        /* Set up write transaction */
        CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_WR_ADDR;
        CodecI2CM_mstrWrBufIndexTmp = 0u;
        CodecI2CM_mstrWrBufIndex    = 0u;
        CodecI2CM_mstrWrBufSize     = cnt;
        CodecI2CM_mstrWrBufPtr      = (volatile uint8 *) wrData;
        CodecI2CM_mstrControl       = (uint8) mode;

        slaveAddress = CodecI2CM_GET_I2C_8BIT_ADDRESS(slaveAddress);

        CodecI2CM_mstrStatus &= (uint16) ~CodecI2CM_I2C_MSTAT_WR_CMPLT;

        CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_ALL);
        CodecI2CM_ClearTxInterruptSource(CodecI2CM_INTR_TX_UNDERFLOW);

        /* The TX and RX FIFO have to be EMPTY */

        /* Enable interrupt source to catch when address is sent */
        CodecI2CM_SetTxInterruptMode(CodecI2CM_INTR_TX_UNDERFLOW);

        /* Generate Start or ReStart */
        if(CodecI2CM_CHECK_I2C_MODE_RESTART(mode))
        {
            CodecI2CM_I2C_MASTER_GENERATE_RESTART;
            CodecI2CM_TX_FIFO_WR_REG = slaveAddress;
        }
        else
        {
            CodecI2CM_TX_FIFO_WR_REG = slaveAddress;
            CodecI2CM_I2C_MASTER_GENERATE_START;
        }
    }

    CodecI2CM_EnableInt();   /* Release lock */

    return(errStatus);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterReadBuf
********************************************************************************
*
* Summary:
*  Automatically reads an entire buffer of data from a slave device.
*  Once the data transfer is initiated by this function, further data transfer
*  is handled by the included ISR.
* Enables the I2C interrupt and clears SCB_ I2C_MSTAT_RD_CMPLT status.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer where to put data from slave.
*  cnt:       Size of buffer to read.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Error status.
*
* Global variables:
*  CodecI2CM_mstrStatus  - used to store current status of I2C Master.
*  CodecI2CM_state       - used to store current state of software FSM.
*  CodecI2CM_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  CodecI2CM_mstrRdBufPtr - used to store pointer to master write buffer.
*  CodecI2CM_mstrRdBufIndex - used to current index within master write
*  buffer.
*  CodecI2CM_mstrRdBufSize - used to store master write buffer size.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterReadBuf(uint32 slaveAddress, uint8 * rdData, uint32 cnt, uint32 mode)
{
    uint32 errStatus;

    errStatus = CodecI2CM_I2C_MSTR_NOT_READY;

    if(NULL != rdData)
    {
        /* Check FSM state and bus before generating Start/ReStart condition */
        if(CodecI2CM_CHECK_I2C_FSM_IDLE)
        {
            CodecI2CM_DisableInt();  /* Lock from interruption */

            /* Check bus state */
            errStatus = CodecI2CM_CHECK_I2C_STATUS(CodecI2CM_I2C_STATUS_BUS_BUSY) ?
                            CodecI2CM_I2C_MSTR_BUS_BUSY : CodecI2CM_I2C_MSTR_NO_ERROR;
        }
        else if(CodecI2CM_CHECK_I2C_FSM_HALT)
        {
            CodecI2CM_mstrStatus &= (uint16) ~CodecI2CM_I2C_MSTAT_XFER_HALT;
                              errStatus  =  CodecI2CM_I2C_MSTR_NO_ERROR;
        }
        else
        {
            /* Unexpected FSM state: exit */
        }
    }

    /* Check master ready to proceed */
    if(CodecI2CM_I2C_MSTR_NO_ERROR == errStatus) /* No error proceed */
    {
        #if (!CodecI2CM_CY_SCBIP_V0 && \
        CodecI2CM_I2C_MULTI_MASTER_SLAVE_CONST && CodecI2CM_I2C_WAKE_ENABLE_CONST)
            CodecI2CM_I2CMasterDisableEcAm();
        #endif /* (!CodecI2CM_CY_SCBIP_V0) */

        /* Set up read transaction */
        CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_RD_ADDR;
        CodecI2CM_mstrRdBufIndex = 0u;
        CodecI2CM_mstrRdBufSize  = cnt;
        CodecI2CM_mstrRdBufPtr   = (volatile uint8 *) rdData;
        CodecI2CM_mstrControl    = (uint8) mode;

        slaveAddress = (CodecI2CM_GET_I2C_8BIT_ADDRESS(slaveAddress) | CodecI2CM_I2C_READ_FLAG);

        CodecI2CM_mstrStatus &= (uint16) ~CodecI2CM_I2C_MSTAT_RD_CMPLT;

        CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_ALL);

        /* TX and RX FIFO have to be EMPTY */

        /* Prepare reading */
        if(CodecI2CM_mstrRdBufSize < CodecI2CM_I2C_FIFO_SIZE)
        {
            /* Reading byte-by-byte */
            CodecI2CM_SetRxInterruptMode(CodecI2CM_INTR_RX_NOT_EMPTY);
        }
        else
        {
            /* Receive RX FIFO chunks */
            CodecI2CM_ENABLE_MASTER_AUTO_DATA_ACK;
            CodecI2CM_SetRxInterruptMode(CodecI2CM_INTR_RX_FULL);
        }

        /* Generate Start or ReStart */
        if(CodecI2CM_CHECK_I2C_MODE_RESTART(mode))
        {
            CodecI2CM_I2C_MASTER_GENERATE_RESTART;
            CodecI2CM_TX_FIFO_WR_REG = (slaveAddress);
        }
        else
        {
            CodecI2CM_TX_FIFO_WR_REG = (slaveAddress);
            CodecI2CM_I2C_MASTER_GENERATE_START;
        }
    }

    CodecI2CM_EnableInt();   /* Release lock */

    return(errStatus);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterSendStart
********************************************************************************
*
* Summary:
*  Generates Start condition and sends slave address with read/write bit.
*  Disables the I2C interrupt.
*  This function is blocking and does not return until start condition and
*  address byte are sent and ACK/NACK response is received or errors occurred.
*
* Parameters:
*  slaveAddress: Right justified 7-bit Slave address (valid range 8 to 120).
*  bitRnW:       Direction of the following transfer. It is defined by
*                read/write bit within address byte.
*
* Return:
*  Error status.
*
* Global variables:
*  CodecI2CM_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterSendStart(uint32 slaveAddress, uint32 bitRnW)
{
    uint32  resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = CodecI2CM_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating Start condition */
    if(CodecI2CM_CHECK_I2C_FSM_IDLE)
    {
        /* If bus is free, generate Start condition */
        if(CodecI2CM_CHECK_I2C_STATUS(CodecI2CM_I2C_STATUS_BUS_BUSY))
        {
            errStatus = CodecI2CM_I2C_MSTR_BUS_BUSY;
        }
        else
        {
            CodecI2CM_DisableInt();  /* Lock from interruption */

        #if (!CodecI2CM_CY_SCBIP_V0 && \
            CodecI2CM_I2C_MULTI_MASTER_SLAVE_CONST && CodecI2CM_I2C_WAKE_ENABLE_CONST)
            CodecI2CM_I2CMasterDisableEcAm();
        #endif /* (!CodecI2CM_CY_SCBIP_V0) */

            slaveAddress = CodecI2CM_GET_I2C_8BIT_ADDRESS(slaveAddress);

            if(0u == bitRnW) /* Write direction */
            {
                CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_WR_DATA;
            }
            else /* Read direction */
            {
                CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_RD_DATA;
                         slaveAddress |= CodecI2CM_I2C_READ_FLAG;
            }

            /* TX and RX FIFO have to be EMPTY */

            CodecI2CM_TX_FIFO_WR_REG = slaveAddress; /* Put address in TX FIFO */
            CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_ALL);

            CodecI2CM_I2C_MASTER_GENERATE_START;


            while(!CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ACK      |
                                                      CodecI2CM_INTR_MASTER_I2C_NACK     |
                                                      CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                      CodecI2CM_INTR_MASTER_I2C_BUS_ERROR))
            {
                /*
                * Write: wait until address has been transferred
                * Read : wait until address has been transferred, data byte is going to RX FIFO as well.
                */
            }

            /* Check the results of the address phase */
            if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ACK))
            {
                errStatus = CodecI2CM_I2C_MSTR_NO_ERROR;
            }
            else if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_NACK))
            {
                errStatus = CodecI2CM_I2C_MSTR_ERR_LB_NAK;
            }
            else if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ARB_LOST))
            {
                CodecI2CM_state = CodecI2CM_I2C_FSM_IDLE;
                             errStatus = CodecI2CM_I2C_MSTR_ERR_ARB_LOST;
                             resetIp   = CodecI2CM_I2C_RESET_ERROR;
            }
            else /* CodecI2CM_INTR_MASTER_I2C_BUS_ERROR set is else condition */
            {
                CodecI2CM_state = CodecI2CM_I2C_FSM_IDLE;
                             errStatus = CodecI2CM_I2C_MSTR_ERR_BUS_ERR;
                             resetIp   = CodecI2CM_I2C_RESET_ERROR;
            }

            CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_I2C_ACK      |
                                                        CodecI2CM_INTR_MASTER_I2C_NACK     |
                                                        CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                        CodecI2CM_INTR_MASTER_I2C_BUS_ERROR);

            /* Reset block in case of: LOST_ARB or BUS_ERR */
            if(0u != resetIp)
            {
                CodecI2CM_SCB_SW_RESET;
            }
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterSendRestart
********************************************************************************
*
* Summary:
*  Generates Restart condition and sends slave address with read/write bit.
*  This function is blocking and does not return until start condition and
*  address are sent and ACK/NACK response is received or errors occurred.
*
* Parameters:
*  slaveAddress: Right justified 7-bit Slave address (valid range 8 to 120).
*  bitRnW:       Direction of the following transfer. It is defined by
*                read/write bit within address byte.
*
* Return:
*  Error status
*
*
* Global variables:
*  CodecI2CM_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterSendRestart(uint32 slaveAddress, uint32 bitRnW)
{
    uint32 resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = CodecI2CM_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating ReStart condition */
    if(CodecI2CM_CHECK_I2C_MASTER_ACTIVE)
    {
        slaveAddress = CodecI2CM_GET_I2C_8BIT_ADDRESS(slaveAddress);

        if(0u == bitRnW) /* Write direction */
        {
            CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_WR_DATA;
        }
        else  /* Read direction */
        {
            CodecI2CM_state  = CodecI2CM_I2C_FSM_MSTR_RD_DATA;
                      slaveAddress |= CodecI2CM_I2C_READ_FLAG;
        }

        /* TX and RX FIFO have to be EMPTY */

        /* Clean-up interrupt status */
        CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_ALL);

        /* A proper ReStart sequence is: generate ReStart, then put an address byte in the TX FIFO.
        * Otherwise the master treats the address in the TX FIFO as a data byte if a previous transfer is write.
        * The write transfer continues instead of ReStart.
        */
        CodecI2CM_I2C_MASTER_GENERATE_RESTART;

        while(CodecI2CM_CHECK_I2C_MASTER_CMD(CodecI2CM_I2C_MASTER_CMD_M_START))
        {
            /* Wait until ReStart has been generated */
        }

        /* Put address into TX FIFO */
        CodecI2CM_TX_FIFO_WR_REG = slaveAddress;

        /* Wait for address to be transferred */
        while(!CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ACK      |
                                                  CodecI2CM_INTR_MASTER_I2C_NACK     |
                                                  CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                  CodecI2CM_INTR_MASTER_I2C_BUS_ERROR))
        {
            /* Wait until address has been transferred */
        }

        /* Check results of address phase */
        if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ACK))
        {
            errStatus = CodecI2CM_I2C_MSTR_NO_ERROR;
        }
        else if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_NACK))
        {
             errStatus = CodecI2CM_I2C_MSTR_ERR_LB_NAK;
        }
        else if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ARB_LOST))
        {
            CodecI2CM_state = CodecI2CM_I2C_FSM_IDLE;
                         errStatus = CodecI2CM_I2C_MSTR_ERR_ARB_LOST;
                         resetIp   = CodecI2CM_I2C_RESET_ERROR;
        }
        else /* CodecI2CM_INTR_MASTER_I2C_BUS_ERROR set is else condition */
        {
            CodecI2CM_state = CodecI2CM_I2C_FSM_IDLE;
                         errStatus = CodecI2CM_I2C_MSTR_ERR_BUS_ERR;
                         resetIp   = CodecI2CM_I2C_RESET_ERROR;
        }

        CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_I2C_ACK      |
                                                    CodecI2CM_INTR_MASTER_I2C_NACK     |
                                                    CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                    CodecI2CM_INTR_MASTER_I2C_BUS_ERROR);

        /* Reset block in case of: LOST_ARB or BUS_ERR */
        if(0u != resetIp)
        {
            CodecI2CM_SCB_SW_RESET;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterSendStop
********************************************************************************
*
* Summary:
*  Generates Stop condition on the bus.
*  At least one byte has to be read if start or restart condition with read
*  direction was generated before.
*  This function is blocking and does not return until a stop condition
*  is generated or error occurred.
*
* Parameters:
*  None
*
* Return:
*  Error status
*
* Side Effects:
*  A valid Start or ReStart condition must be generated before calling
*  this function. This function does nothing if Start or ReStart condition
*  failed before this function was called.
*  For read transfer, at least one byte has to be read before Stop generation.
*
* Global variables:
*  CodecI2CM_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterSendStop(void)
{
    uint32 resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = CodecI2CM_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating Stop condition */
    if(CodecI2CM_CHECK_I2C_MASTER_ACTIVE)
    {
        /*
        * Write direction: generates Stop
        * Read  direction: generates NACK and Stop
        */
        CodecI2CM_I2C_MASTER_GENERATE_STOP;

        while(!CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_STOP     |
                                                  CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                  CodecI2CM_INTR_MASTER_I2C_BUS_ERROR))
        {
            /* Wait until Stop has been generated */
        }

        /* Check Stop generation */
        if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_STOP))
        {
            errStatus = CodecI2CM_I2C_MSTR_NO_ERROR;
        }
        else if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ARB_LOST))
        {
            errStatus = CodecI2CM_I2C_MSTR_ERR_ARB_LOST;
            resetIp   = CodecI2CM_I2C_RESET_ERROR;
        }
        else /* CodecI2CM_INTR_MASTER_I2C_BUS_ERROR is set */
        {
            errStatus = CodecI2CM_I2C_MSTR_ERR_BUS_ERR;
            resetIp   = CodecI2CM_I2C_RESET_ERROR;
        }

        CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_I2C_STOP     |
                                                    CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                    CodecI2CM_INTR_MASTER_I2C_BUS_ERROR);

        CodecI2CM_state = CodecI2CM_I2C_FSM_IDLE;

        /* Reset block in case of: LOST_ARB or BUS_ERR */
        if(0u != resetIp)
        {
            CodecI2CM_SCB_SW_RESET;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterWriteByte
********************************************************************************
*
* Summary:
*  Sends one byte to a slave.
*  This function is blocking and does not return until byte is transmitted
*  or error occurred.
*
* Parameters:
*  data: The data byte to send to the slave.
*
* Return:
*  Error status
*
* Side Effects:
*  A valid Start or ReStart condition must be generated before calling
*  this function. This function does nothing if Start or ReStart condition
*  failed before this function was called.
*
* Global variables:
*  CodecI2CM_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterWriteByte(uint32 theByte)
{
    uint32 resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = CodecI2CM_I2C_MSTR_NOT_READY;

    /* Check FSM state before write byte */
    if(CodecI2CM_CHECK_I2C_MASTER_ACTIVE)
    {
        CodecI2CM_TX_FIFO_WR_REG = theByte;

        while(!CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ACK      |
                                                  CodecI2CM_INTR_MASTER_I2C_NACK     |
                                                  CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                  CodecI2CM_INTR_MASTER_I2C_BUS_ERROR))
        {
            /* Wait until byte has been transferred */
        }

        /* Check results after byte was sent */
        if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ACK))
        {
            CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_HALT;
                         errStatus = CodecI2CM_I2C_MSTR_NO_ERROR;
        }
        else if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_NACK))
        {
            CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_HALT;
                         errStatus = CodecI2CM_I2C_MSTR_ERR_LB_NAK;
        }
        else if(CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ARB_LOST))
        {
            CodecI2CM_state = CodecI2CM_I2C_FSM_IDLE;
                         errStatus = CodecI2CM_I2C_MSTR_ERR_ARB_LOST;
                         resetIp   = CodecI2CM_I2C_RESET_ERROR;
        }
        else /* CodecI2CM_INTR_MASTER_I2C_BUS_ERROR set is */
        {
            CodecI2CM_state = CodecI2CM_I2C_FSM_IDLE;
                         errStatus = CodecI2CM_I2C_MSTR_ERR_BUS_ERR;
                         resetIp   = CodecI2CM_I2C_RESET_ERROR;
        }

        CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_I2C_ACK      |
                                                    CodecI2CM_INTR_MASTER_I2C_NACK     |
                                                    CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                    CodecI2CM_INTR_MASTER_I2C_BUS_ERROR);

        /* Reset block in case of: LOST_ARB or BUS_ERR */
        if(0u != resetIp)
        {
            CodecI2CM_SCB_SW_RESET;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterReadByte
********************************************************************************
*
* Summary:
*  Reads one byte from a slave and ACKs or NAKs received byte.
*  This function does not generate NAK explicitly. The following call
*  SCB_I2CMasterSendStop() or SCB_I2CMasterSendRestart() will generate NAK and
*  Stop or ReStart condition appropriately.
*  This function is blocking and does not return until byte is received
*  or error occurred.
*
* Parameters:
*  ackNack: Response to received byte.
*
* Return:
*  Byte read from the slave. In case of error the MSB of returned data
*  is set to 1.
*
* Side Effects:
*  A valid Start or ReStart condition must be generated before calling this
*  function. This function does nothing and returns invalid byte value
*  if Start or ReStart conditions failed before this function was called.
*
* Global variables:
*  CodecI2CM_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterReadByte(uint32 ackNack)
{
    uint32 theByte;

    /* Return invalid byte in case BUS_ERR happen during receiving */
    theByte = CodecI2CM_I2C_INVALID_BYTE;

    /* Check FSM state before read byte */
    if(CodecI2CM_CHECK_I2C_MASTER_ACTIVE)
    {
        while((!CodecI2CM_CHECK_INTR_RX(CodecI2CM_INTR_RX_NOT_EMPTY)) &&
              (!CodecI2CM_CHECK_INTR_MASTER(CodecI2CM_INTR_MASTER_I2C_ARB_LOST |
                                                  CodecI2CM_INTR_MASTER_I2C_BUS_ERROR)))
        {
            /* Wait until byte has been received */
        }

        /* Check the results after the byte was sent */
        if(CodecI2CM_CHECK_INTR_RX(CodecI2CM_INTR_RX_NOT_EMPTY))
        {
            theByte = CodecI2CM_RX_FIFO_RD_REG;

            CodecI2CM_ClearRxInterruptSource(CodecI2CM_INTR_RX_NOT_EMPTY);

            if(0u == ackNack)
            {
                CodecI2CM_I2C_MASTER_GENERATE_ACK;
            }
            else
            {
                /* NACK is generated by Stop or ReStart command */
                CodecI2CM_state = CodecI2CM_I2C_FSM_MSTR_HALT;
            }
        }
        else
        {
            CodecI2CM_ClearMasterInterruptSource(CodecI2CM_INTR_MASTER_ALL);

            /* Reset block in case of: LOST_ARB or BUS_ERR */
            CodecI2CM_SCB_SW_RESET;
        }
    }

    return(theByte);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterGetReadBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes that has been transferred with an
*  SCB_I2CMasterReadBuf() function.
*
* Parameters:
*  None
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it returns
*  the byte count transferred so far.
*
* Side Effects:
*  This function returns not valid value if SCB_I2C_MSTAT_ERR_ARB_LOST
*  or SCB_I2C_MSTAT_ERR_BUS_ERROR occurred while read transfer.
*
* Global variables:
*  CodecI2CM_mstrRdBufIndex - used to current index within master read
*  buffer.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterGetReadBufSize(void)
{
    return(CodecI2CM_mstrRdBufIndex);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterGetWriteBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes that have been transferred with an
*  SCB_I2CMasterWriteBuf() function.
*
* Parameters:
*  None
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it returns
*  zero unit transfer completion.
*
* Side Effects:
*  This function returns not valid value if SCB_I2C_MSTAT_ERR_ARB_LOST
*  or SCB_I2C_MSTAT_ERR_BUS_ERROR occurred while read transfer.
*
* Global variables:
*  CodecI2CM_mstrWrBufIndex - used to current index within master write
*  buffer.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterGetWriteBufSize(void)
{
    return(CodecI2CM_mstrWrBufIndex);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterClearReadBuf
********************************************************************************
*
* Summary:
*  Resets the read buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  CodecI2CM_mstrRdBufIndex - used to current index within master read
*   buffer.
*  CodecI2CM_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
void CodecI2CM_I2CMasterClearReadBuf(void)
{
    CodecI2CM_DisableInt();  /* Lock from interruption */

    CodecI2CM_mstrRdBufIndex = 0u;
    CodecI2CM_mstrStatus    &= (uint16) ~CodecI2CM_I2C_MSTAT_RD_CMPLT;

    CodecI2CM_EnableInt();   /* Release lock */
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterClearWriteBuf
********************************************************************************
*
* Summary:
*  Resets the write buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  CodecI2CM_mstrRdBufIndex - used to current index within master read
*   buffer.
*  CodecI2CM_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
void CodecI2CM_I2CMasterClearWriteBuf(void)
{
    CodecI2CM_DisableInt();  /* Lock from interruption */

    CodecI2CM_mstrWrBufIndex = 0u;
    CodecI2CM_mstrStatus    &= (uint16) ~CodecI2CM_I2C_MSTAT_WR_CMPLT;

    CodecI2CM_EnableInt();   /* Release lock */
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterStatus
********************************************************************************
*
* Summary:
*  Returns the master's communication status.
*
* Parameters:
*  None
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  CodecI2CM_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterStatus(void)
{
    uint32 status;

    CodecI2CM_DisableInt();  /* Lock from interruption */

    status = (uint32) CodecI2CM_mstrStatus;

    if (CodecI2CM_CHECK_I2C_MASTER_ACTIVE)
    {
        /* Add status of master pending transaction: MSTAT_XFER_INP */
        status |= (uint32) CodecI2CM_I2C_MSTAT_XFER_INP;
    }

    CodecI2CM_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CMasterClearStatus
********************************************************************************
*
* Summary:
*  Clears all status flags and returns the master status.
*
* Parameters:
*  None
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  CodecI2CM_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
uint32 CodecI2CM_I2CMasterClearStatus(void)
{
    uint32 status;

    CodecI2CM_DisableInt();  /* Lock from interruption */

    /* Read and clear master status */
    status = (uint32) CodecI2CM_mstrStatus;
    CodecI2CM_mstrStatus = CodecI2CM_I2C_MSTAT_CLEAR;

    CodecI2CM_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: CodecI2CM_I2CReStartGeneration
********************************************************************************
*
* Summary:
*  Generates a ReStart condition:
*  SCB IP V1 and later: Generates ReStart using the scb IP functionality
*    Sets the I2C_MASTER_CMD_M_START and I2C_MASTER_CMD_M_NACK (if the previous
*    transaction was read) bits in the SCB.I2C_MASTER_CMD register.
*    This combination forces the master to generate ReStart.
*
*  SCB IP V0: Generates Restart using the GPIO and scb IP functionality.
*   After the master completes write or read, the SCL is stretched.
*   The master waits until SDA line is released by the slave. Then the GPIO
*   function is enabled and the scb IP disabled as it already does not drive
*   the bus. In case of the previous transfer was read, the NACK is generated
*   by the GPIO. The delay of tLOW is added to manage the hold time.
*   Set I2C_M_CMD.START and enable the scb IP. The ReStart generation
*   is started after the I2C function is enabled for the SCL.
*   Note1: the scb IP due re-enable generates Start but on the I2C bus it
*          appears as ReStart.
*   Note2: the I2C_M_CMD.START is queued if scb IP is disabled.
*   Note3: the I2C_STATUS_M_READ is cleared is address was NACKed before.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  SCB IP V0: The NACK generation by the GPIO may cause a greater SCL period
*             than expected for the selected master data rate.
*
*******************************************************************************/
void CodecI2CM_I2CReStartGeneration(void)
{
#if(CodecI2CM_CY_SCBIP_V0)
    /* Generates Restart use GPIO and scb IP functionality. Ticket ID#143715,
    ID#145238 and ID#173656 */
    uint32 status = CodecI2CM_I2C_STATUS_REG;

    while(CodecI2CM_WAIT_SDA_SET_HIGH)
    {
        /* Wait when slave release SDA line: SCL tHIGH is complete */
    }

    /* Prepare DR register to drive SCL line */
    CodecI2CM_SET_I2C_SCL_DR(CodecI2CM_I2C_SCL_LOW);

    /* Switch HSIOM to GPIO: SCL goes low */
    CodecI2CM_SET_I2C_SCL_HSIOM_SEL(CodecI2CM_HSIOM_GPIO_SEL);

    /* Disable SCB block */
    CodecI2CM_CTRL_REG &= (uint32) ~CodecI2CM_CTRL_ENABLED;

    if(0u != (status & CodecI2CM_I2C_STATUS_M_READ))
    {
        /* Generate NACK use GPIO functionality */
        CodecI2CM_SET_I2C_SCL_DR(CodecI2CM_I2C_SCL_LOW);
        CyDelayUs(CodecI2CM_I2C_TLOW_TIME); /* Count tLOW */

        CodecI2CM_SET_I2C_SCL_DR(CodecI2CM_I2C_SCL_HIGH);
        while(CodecI2CM_WAIT_SCL_SET_HIGH)
        {
            /* Wait until slave releases SCL in case if it stretches */
        }
        CyDelayUs(CodecI2CM_I2C_THIGH_TIME); /* Count tHIGH */
    }

    /* Count tLOW as hold time for write and read */
    CodecI2CM_SET_I2C_SCL_DR(CodecI2CM_I2C_SCL_LOW);
    CyDelayUs(CodecI2CM_I2C_TLOW_TIME); /* Count tLOW */

    /* Set command for Start generation: it will appear */
    CodecI2CM_I2C_MASTER_CMD_REG = CodecI2CM_I2C_MASTER_CMD_M_START;

    /* Enable SCB block */
    CodecI2CM_CTRL_REG |= (uint32) CodecI2CM_CTRL_ENABLED;

    /* Switch HSIOM to I2C: */
    CodecI2CM_SET_I2C_SCL_HSIOM_SEL(CodecI2CM_HSIOM_I2C_SEL);

    /* Revert SCL DR register */
    CodecI2CM_SET_I2C_SCL_DR(CodecI2CM_I2C_SCL_HIGH);
#else
    uint32 cmd;

    /* Generates ReStart use scb IP functionality */
    cmd  = CodecI2CM_I2C_MASTER_CMD_M_START;
    cmd |= CodecI2CM_CHECK_I2C_STATUS(CodecI2CM_I2C_STATUS_M_READ) ?
                (CodecI2CM_I2C_MASTER_CMD_M_NACK) : (0u);

    CodecI2CM_I2C_MASTER_CMD_REG = cmd;
#endif /* (CodecI2CM_CY_SCBIP_V1) */
}

#endif /* (CodecI2CM_I2C_MASTER_CONST) */


#if (!CodecI2CM_CY_SCBIP_V0 && \
    CodecI2CM_I2C_MULTI_MASTER_SLAVE_CONST && CodecI2CM_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: CodecI2CM_I2CMasterDisableEcAm
    ********************************************************************************
    *
    * Summary:
    *  Disables externally clocked address match to enable master operation
    *  in active mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    static void CodecI2CM_I2CMasterDisableEcAm(void)
    {
        /* Disables externally clocked address match to enable master operation in active mode.
        * This applicable only for Multi-Master-Slave with wakeup enabled. Ticket ID#192742 */
        if (0u != (CodecI2CM_CTRL_REG & CodecI2CM_CTRL_EC_AM_MODE))
        {
            /* Enable external address match logic */
            CodecI2CM_Stop();
            CodecI2CM_CTRL_REG &= (uint32) ~CodecI2CM_CTRL_EC_AM_MODE;
            CodecI2CM_Enable();
        }
    }
#endif /* (!CodecI2CM_CY_SCBIP_V0) */


/* [] END OF FILE */
