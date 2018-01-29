/*******************************************************************************
* File Name: Clk_Counter.c
* Version 2.20
*
*  Description:
*   Provides system API for the clocking, interrupts and watchdog timer.
*
*  Note:
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <cydevice_trm.h>
#include "Clk_Counter.h"

#if defined CYREG_PERI_DIV_CMD

/*******************************************************************************
* Function Name: Clk_Counter_StartEx
********************************************************************************
*
* Summary:
*  Starts the clock, aligned to the specified running clock.
*
* Parameters:
*  alignClkDiv:  The divider to which phase alignment is performed when the
*    clock is started.
*
* Returns:
*  None
*
*******************************************************************************/
void Clk_Counter_StartEx(uint32 alignClkDiv)
{
    /* Make sure any previous start command has finished. */
    while((Clk_Counter_CMD_REG & Clk_Counter_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and enable. */
    Clk_Counter_CMD_REG =
        ((uint32)Clk_Counter__DIV_ID << Clk_Counter_CMD_DIV_SHIFT)|
        (alignClkDiv << Clk_Counter_CMD_PA_DIV_SHIFT) |
        (uint32)Clk_Counter_CMD_ENABLE_MASK;
}

#else

/*******************************************************************************
* Function Name: Clk_Counter_Start
********************************************************************************
*
* Summary:
*  Starts the clock.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/

void Clk_Counter_Start(void)
{
    /* Set the bit to enable the clock. */
    Clk_Counter_ENABLE_REG |= Clk_Counter__ENABLE_MASK;
}

#endif /* CYREG_PERI_DIV_CMD */


/*******************************************************************************
* Function Name: Clk_Counter_Stop
********************************************************************************
*
* Summary:
*  Stops the clock and returns immediately. This API does not require the
*  source clock to be running but may return before the hardware is actually
*  disabled.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/
void Clk_Counter_Stop(void)
{
#if defined CYREG_PERI_DIV_CMD

    /* Make sure any previous start command has finished. */
    while((Clk_Counter_CMD_REG & Clk_Counter_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and disable. */
    Clk_Counter_CMD_REG =
        ((uint32)Clk_Counter__DIV_ID << Clk_Counter_CMD_DIV_SHIFT)|
        ((uint32)Clk_Counter_CMD_DISABLE_MASK);

#else

    /* Clear the bit to disable the clock. */
    Clk_Counter_ENABLE_REG &= (uint32)(~Clk_Counter__ENABLE_MASK);
    
#endif /* CYREG_PERI_DIV_CMD */
}


/*******************************************************************************
* Function Name: Clk_Counter_SetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Modifies the clock divider and the fractional divider.
*
* Parameters:
*  clkDivider:  Divider register value (0-65535). This value is NOT the
*    divider; the clock hardware divides by clkDivider plus one. For example,
*    to divide the clock by 2, this parameter should be set to 1.
*  fracDivider:  Fractional Divider register value (0-31).
* Returns:
*  None
*
*******************************************************************************/
void Clk_Counter_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional)
{
    uint32 maskVal;
    uint32 regVal;
    
#if defined (Clk_Counter__FRAC_MASK) || defined (CYREG_PERI_DIV_CMD)
    
	/* get all but divider bits */
    maskVal = Clk_Counter_DIV_REG & 
                    (uint32)(~(uint32)(Clk_Counter_DIV_INT_MASK | Clk_Counter_DIV_FRAC_MASK)); 
	/* combine mask and new divider vals into 32-bit value */
    regVal = maskVal |
        ((uint32)((uint32)clkDivider <<  Clk_Counter_DIV_INT_SHIFT) & Clk_Counter_DIV_INT_MASK) |
        ((uint32)((uint32)clkFractional << Clk_Counter_DIV_FRAC_SHIFT) & Clk_Counter_DIV_FRAC_MASK);
    
#else
    /* get all but integer divider bits */
    maskVal = Clk_Counter_DIV_REG & (uint32)(~(uint32)Clk_Counter__DIVIDER_MASK);
    /* combine mask and new divider val into 32-bit value */
    regVal = clkDivider | maskVal;
    
#endif /* Clk_Counter__FRAC_MASK || CYREG_PERI_DIV_CMD */

    Clk_Counter_DIV_REG = regVal;
}


/*******************************************************************************
* Function Name: Clk_Counter_GetDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Divide value of the clock minus 1. For example, if the clock is set to
*  divide by 2, the return value will be 1.
*
*******************************************************************************/
uint16 Clk_Counter_GetDividerRegister(void)
{
    return (uint16)((Clk_Counter_DIV_REG & Clk_Counter_DIV_INT_MASK)
        >> Clk_Counter_DIV_INT_SHIFT);
}


/*******************************************************************************
* Function Name: Clk_Counter_GetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock fractional divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Fractional Divide value of the clock
*  0 if the fractional divider is not in use.
*
*******************************************************************************/
uint8 Clk_Counter_GetFractionalDividerRegister(void)
{
#if defined (Clk_Counter__FRAC_MASK)
    /* return fractional divider bits */
    return (uint8)((Clk_Counter_DIV_REG & Clk_Counter_DIV_FRAC_MASK)
        >> Clk_Counter_DIV_FRAC_SHIFT);
#else
    return 0u;
#endif /* Clk_Counter__FRAC_MASK */
}


/* [] END OF FILE */
