/*******************************************************************************
* File Name: CodecI2CM_BOOT.c
* Version 3.10
*
* Description:
*  This file provides the source code of the bootloader communication APIs
*  for the SCB Component Unconfigured mode.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CodecI2CM_BOOT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (CodecI2CM_BTLDR_COMM_ENABLED) && \
                                (CodecI2CM_SCB_MODE_UNCONFIG_CONST_CFG)

/*******************************************************************************
* Function Name: CodecI2CM_CyBtldrCommStart
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommStart function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CodecI2CM_CyBtldrCommStart(void)
{
    if (CodecI2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        CodecI2CM_I2CCyBtldrCommStart();
    }
    else if (CodecI2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        CodecI2CM_EzI2CCyBtldrCommStart();
    }
#if (!CodecI2CM_CY_SCBIP_V1)
    else if (CodecI2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        CodecI2CM_SpiCyBtldrCommStart();
    }
    else if (CodecI2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        CodecI2CM_UartCyBtldrCommStart();
    }
#endif /* (!CodecI2CM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: CodecI2CM_CyBtldrCommStop
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommStop function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CodecI2CM_CyBtldrCommStop(void)
{
    if (CodecI2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        CodecI2CM_I2CCyBtldrCommStop();
    }
    else if (CodecI2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        CodecI2CM_EzI2CCyBtldrCommStop();
    }
#if (!CodecI2CM_CY_SCBIP_V1)
    else if (CodecI2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        CodecI2CM_SpiCyBtldrCommStop();
    }
    else if (CodecI2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        CodecI2CM_UartCyBtldrCommStop();
    }
#endif /* (!CodecI2CM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: CodecI2CM_CyBtldrCommReset
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommReset function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CodecI2CM_CyBtldrCommReset(void)
{
    if(CodecI2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        CodecI2CM_I2CCyBtldrCommReset();
    }
    else if(CodecI2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        CodecI2CM_EzI2CCyBtldrCommReset();
    }
#if (!CodecI2CM_CY_SCBIP_V1)
    else if(CodecI2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        CodecI2CM_SpiCyBtldrCommReset();
    }
    else if(CodecI2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        CodecI2CM_UartCyBtldrCommReset();
    }
#endif /* (!CodecI2CM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: CodecI2CM_CyBtldrCommRead
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommRead function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  pData:    Pointer to storage for the block of data to be read from the
*            bootloader host
*  size:     Number of bytes to be read.
*  count:    Pointer to the variable to write the number of bytes actually
*            read.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
*******************************************************************************/
cystatus CodecI2CM_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(CodecI2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = CodecI2CM_I2CCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(CodecI2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = CodecI2CM_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
    }
#if (!CodecI2CM_CY_SCBIP_V1)
    else if(CodecI2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = CodecI2CM_SpiCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(CodecI2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        status = CodecI2CM_UartCyBtldrCommRead(pData, size, count, timeOut);
    }
#endif /* (!CodecI2CM_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}


/*******************************************************************************
* Function Name: CodecI2CM_CyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommWrite  function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  pData:    Pointer to the block of data to be written to the bootloader host.
*  size:     Number of bytes to be written.
*  count:    Pointer to the variable to write the number of bytes actually
*            written.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
*******************************************************************************/
cystatus CodecI2CM_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(CodecI2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = CodecI2CM_I2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(CodecI2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = CodecI2CM_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
#if (!CodecI2CM_CY_SCBIP_V1)
    else if(CodecI2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = CodecI2CM_SpiCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(CodecI2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        status = CodecI2CM_UartCyBtldrCommWrite(pData, size, count, timeOut);
    }
#endif /* (!CodecI2CM_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (CodecI2CM_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
