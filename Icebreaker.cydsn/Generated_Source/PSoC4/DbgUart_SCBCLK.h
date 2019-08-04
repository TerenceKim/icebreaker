/*******************************************************************************
* File Name: DbgUart_SCBCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_DbgUart_SCBCLK_H)
#define CY_CLOCK_DbgUart_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void DbgUart_SCBCLK_StartEx(uint32 alignClkDiv);
#define DbgUart_SCBCLK_Start() \
    DbgUart_SCBCLK_StartEx(DbgUart_SCBCLK__PA_DIV_ID)

#else

void DbgUart_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void DbgUart_SCBCLK_Stop(void);

void DbgUart_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 DbgUart_SCBCLK_GetDividerRegister(void);
uint8  DbgUart_SCBCLK_GetFractionalDividerRegister(void);

#define DbgUart_SCBCLK_Enable()                         DbgUart_SCBCLK_Start()
#define DbgUart_SCBCLK_Disable()                        DbgUart_SCBCLK_Stop()
#define DbgUart_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    DbgUart_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define DbgUart_SCBCLK_SetDivider(clkDivider)           DbgUart_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define DbgUart_SCBCLK_SetDividerValue(clkDivider)      DbgUart_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define DbgUart_SCBCLK_DIV_ID     DbgUart_SCBCLK__DIV_ID

#define DbgUart_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define DbgUart_SCBCLK_CTRL_REG   (*(reg32 *)DbgUart_SCBCLK__CTRL_REGISTER)
#define DbgUart_SCBCLK_DIV_REG    (*(reg32 *)DbgUart_SCBCLK__DIV_REGISTER)

#define DbgUart_SCBCLK_CMD_DIV_SHIFT          (0u)
#define DbgUart_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define DbgUart_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define DbgUart_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define DbgUart_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << DbgUart_SCBCLK_CMD_DISABLE_SHIFT))
#define DbgUart_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << DbgUart_SCBCLK_CMD_ENABLE_SHIFT))

#define DbgUart_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define DbgUart_SCBCLK_DIV_FRAC_SHIFT (3u)
#define DbgUart_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define DbgUart_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define DbgUart_SCBCLK_DIV_REG        (*(reg32 *)DbgUart_SCBCLK__REGISTER)
#define DbgUart_SCBCLK_ENABLE_REG     DbgUart_SCBCLK_DIV_REG
#define DbgUart_SCBCLK_DIV_FRAC_MASK  DbgUart_SCBCLK__FRAC_MASK
#define DbgUart_SCBCLK_DIV_FRAC_SHIFT (16u)
#define DbgUart_SCBCLK_DIV_INT_MASK   DbgUart_SCBCLK__DIVIDER_MASK
#define DbgUart_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_DbgUart_SCBCLK_H) */

/* [] END OF FILE */
