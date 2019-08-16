
/*******************************************************************************
* File Name: cyfitter_cfg.c
* 
* PSoC Creator  4.2
*
* Description:
* This file contains device initialization code.
* Except for the user defined sections in CyClockStartupError(), this file should not be modified.
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include "cytypes.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "CyLFClk.h"
#include "cyfitter_cfg.h"
#include "cyapicallbacks.h"


#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    #define CYPACKED 
    #define CYPACKED_ATTR __attribute__ ((packed))
    #define CYALIGNED __attribute__ ((aligned))
    #define CY_CFG_UNUSED __attribute__ ((unused))
    #ifndef CY_CFG_SECTION
        #define CY_CFG_SECTION __attribute__ ((section(".psocinit")))
    #endif
    
    #if defined(__ARMCC_VERSION)
        #define CY_CFG_MEMORY_BARRIER() __memory_changed()
    #else
        #define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
    #endif
    
#elif defined(__ICCARM__)
    #include <intrinsics.h>

    #define CYPACKED __packed
    #define CYPACKED_ATTR 
    #define CYALIGNED _Pragma("data_alignment=4")
    #define CY_CFG_UNUSED _Pragma("diag_suppress=Pe177")
    #define CY_CFG_SECTION _Pragma("location=\".psocinit\"")
    
    #define CY_CFG_MEMORY_BARRIER() __DMB()
    
#else
    #error Unsupported toolchain
#endif

#ifndef CYCODE
    #define CYCODE
#endif
#ifndef CYDATA
    #define CYDATA
#endif
#ifndef CYFAR
    #define CYFAR
#endif
#ifndef CYXDATA
    #define CYXDATA
#endif


CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n);
CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n)
{
	(void)memset(s, 0, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}




/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u
#define CYCLOCKSTART_FLL_ERROR   4u
#define CYCLOCKSTART_WCO_ERROR   5u


#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

#ifdef CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK
    CY_CFG_Clock_Startup_ErrorCallback();
#else
    /*  If not using CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK, place your clock startup code here. */
    /* `#START CyClockStartupError` */



    /* `#END` */

    while(1) {}
#endif /* CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK */
}
#endif

#define CY_CFG_BASE_ADDR_COUNT 18u
CYPACKED typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED_ATTR cy_cfg_addrvalue_t;



/*******************************************************************************
* Function Name: cfg_write_bytes32
********************************************************************************
* Summary:
*  This function is used for setting up the chip configuration areas that
*  contain relatively sparse data.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[]);
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[])
{
	/* For 32-bit little-endian architectures */
	uint32 i, j = 0u;
	for (i = 0u; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = addr_table[i];
		uint8 count = (uint8)baseAddr;
		baseAddr &= 0xFFFFFF00u;
		while (count != 0u)
		{
			CY_SET_REG8((void *)(baseAddr + data_table[j].offset), data_table[j].value);
			j++;
			count--;
		}
	}
}


/*******************************************************************************
* Function Name: ClockSetup
********************************************************************************
*
* Summary:
*   Performs the initialization of all of the clocks in the device based on the
*   settings in the Clock tab of the DWR.  This includes enabling the requested
*   clocks and setting the necessary dividers to produce the desired frequency. 
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void ClockSetup(void);
CY_CFG_SECTION
static void ClockSetup(void)
{
	uint8 pllTimeout;

	/* Set Flash Cycles based on max possible frequency in case a glitch occurs during ClockSetup(). */
	CY_SET_REG32((void CYXDATA *)(CYREG_CPUSS_FLASH_CTL), (0x0012u));

	/* Start the WCO */
	CySysClkWcoStart();
	CyDelayCycles(12000000u); /* WCO may take up to 500ms to start */

	CySysClkImoEnableUsbLock();
	/* Setup and trim IMO based on desired frequency. */
	CySysClkWriteImoFreq(48u);
	/* CYDEV_CLK_ILO_CONFIG Starting address: CYDEV_CLK_ILO_CONFIG */
	CY_SET_REG32((void *)(CYREG_CLK_ILO_CONFIG), 0x80000006u);

	/* CYDEV_WDT_CONFIG Starting address: CYDEV_WDT_CONFIG */
	CY_SET_REG32((void *)(CYREG_WDT_CONFIG), 0x40000000u);


	/* Configure ECO trim */
	CY_SET_REG32((void CYXDATA *)(CYREG_CLK_ECO_CONFIG), (0x00000000u));
	CY_SET_REG32((void CYXDATA *)(CYREG_CLK_ECO_TRIM0), (0x0000001Fu));
	CY_SET_REG32((void CYXDATA *)(CYREG_CLK_ECO_TRIM1), (0x0000001Au));
	/* Start the ECO and do not check status since it is not needed for HFCLK */
	(void)CySysClkEcoStart(2000u);
	CyDelayUs(1500u); /* Wait to stabilize */
	CY_SET_REG32(CYREG_CLK_SELECT, ((CY_GET_REG32(CYREG_CLK_SELECT) & 0xFFFFFE07u) | 0x00000090u));
	CY_SET_REG32(CYREG_CLK_PLL0_CONFIG, 0x8002060Au);
	CY_SET_REG32(CYREG_CLK_PLL1_CONFIG, 0x80020F15u);
	CyDelayCycles(240u);
	CY_SET_REG32(CYREG_CLK_PLL0_CONFIG, 0xC002060Au);
	CY_SET_REG32(CYREG_CLK_PLL1_CONFIG, 0xC0020F15u);
	for (pllTimeout = 0u; (pllTimeout < 25u) && ((CY_GET_REG32(CYREG_CLK_PLL0_STATUS) == 0u) || (CY_GET_REG32(CYREG_CLK_PLL1_STATUS) == 0u)); pllTimeout++)
	{
		CyDelayCycles(480u);
	}

	/* Setup phase aligned clocks */
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL1, 0x00000300u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF41u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL0, 0x00000000u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF40u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL3, 0x00001700u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF43u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL4, 0x00002200u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF44u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL2, 0x00000500u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF42u);

	/* CYDEV_CLK_IMO_CONFIG Starting address: CYDEV_CLK_IMO_CONFIG */
	CY_SET_REG32((void *)(CYREG_CLK_IMO_CONFIG), 0x82000000u);

	/* CYDEV_CLK_SELECT Starting address: CYDEV_CLK_SELECT */
	CY_SET_REG32((void *)(CYREG_CLK_SELECT), 0x00040090u);

	/* CYDEV_PERI_PCLK_CTL18 Starting address: CYDEV_PERI_PCLK_CTL18 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL18), 0x00000041u);

	/* CYDEV_PERI_PCLK_CTL17 Starting address: CYDEV_PERI_PCLK_CTL17 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL17), 0x00000041u);

	/* CYDEV_PERI_PCLK_CTL16 Starting address: CYDEV_PERI_PCLK_CTL16 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL16), 0x00000041u);

	/* CYDEV_PERI_PCLK_CTL12 Starting address: CYDEV_PERI_PCLK_CTL12 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL12), 0x00000040u);

	/* CYDEV_PERI_PCLK_CTL11 Starting address: CYDEV_PERI_PCLK_CTL11 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL11), 0x00000040u);

	/* CYDEV_PERI_PCLK_CTL10 Starting address: CYDEV_PERI_PCLK_CTL10 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL10), 0x00000043u);

	/* CYDEV_PERI_PCLK_CTL5 Starting address: CYDEV_PERI_PCLK_CTL5 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL5), 0x00000044u);

	/* CYDEV_PERI_PCLK_CTL2 Starting address: CYDEV_PERI_PCLK_CTL2 */
	CY_SET_REG32((void *)(CYREG_PERI_PCLK_CTL2), 0x00000042u);

	(void)CyIntSetVector(8u, &CySysWdtIsr);
	CyIntEnable(8u);
	CY_SET_REG32((void *)(CYREG_WDT_MATCH), 0x001F0000u);
	CY_SET_REG32((void *)(CYREG_WDT_CONFIG), 0x40000000u);
	CY_SET_REG32((void *)(CYREG_WDT_CONTROL), 0x00000800u);
	while ((CY_GET_XTND_REG32((void CYFAR *)(CYREG_WDT_CONTROL)) & 0x00000800u) != 0u) { }
	CY_SET_REG32((void *)(CYREG_WDT_CONTROL), 0x00000100u);
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	CY_SET_XTND_REG32((void CYFAR *)CYREG_HSIOM_AMUX_SPLIT_CTL1, 0x00000033u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_HSIOM_AMUX_SPLIT_CTL2, 0x00000033u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_CTBM0_DFT_CTRL, 0x00000003u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_CTBM1_DFT_CTRL, 0x00000003u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_SAR_CTRL, 0x80000000u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_SAR_MUX_SWITCH0, 0x000C0000u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_SAR_MUX_SWITCH_HW_CTRL, 0x000C0000u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_PASS_DSAB_DSAB_CTRL, 0x00000000u);
	SetAnalogRoutingPumps(1);
}


/*******************************************************************************
* Function Name: SetAnalogRoutingPumps
********************************************************************************
*
* Summary:
* Enables or disables the analog pumps feeding analog routing switches.
* Intended to be called at startup, based on the Vdda system configuration;
* may be called during operation when the user informs us that the Vdda voltage crossed the pump threshold.
*
* Parameters:
*  enabled - 1 to enable the pumps, 0 to disable the pumps
*
* Return:
*  void
*
*******************************************************************************/
void SetAnalogRoutingPumps(uint8 enabled)
{
	uint32 regValue = CY_GET_XTND_REG32((void *)(CYREG_SAR_PUMP_CTRL));
	if (enabled != 0u)
	{
		regValue |= 0x80000000u;
	}
	else
	{
		regValue &= ~0x80000000u;
	}
	CY_SET_XTND_REG32((void *)(CYREG_SAR_PUMP_CTRL), regValue);
}




/*******************************************************************************
* Function Name: cyfitter_cfg
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_SECTION
void cyfitter_cfg(void)
{
	/* Disable interrupts by default. Let user enable if/when they want. */
	CyGlobalIntDisable;

	/* Enable the clock in the interrupt controller for the routed interrupts */
	CY_SET_REG8((void *)CYREG_UDB_UDB_INT_CLK_CTL, 0x01u);
	{
		static const uint32 CYCODE cy_cfg_addr_table[] = {
			0x400F0002u, /* Base address: 0x400F0000 Count: 2 */
			0x400F3050u, /* Base address: 0x400F3000 Count: 80 */
			0x400F3136u, /* Base address: 0x400F3100 Count: 54 */
			0x400F320Au, /* Base address: 0x400F3200 Count: 10 */
			0x400F334Au, /* Base address: 0x400F3300 Count: 74 */
			0x400F343Cu, /* Base address: 0x400F3400 Count: 60 */
			0x400F3548u, /* Base address: 0x400F3500 Count: 72 */
			0x400F3615u, /* Base address: 0x400F3600 Count: 21 */
			0x400F3749u, /* Base address: 0x400F3700 Count: 73 */
			0x400F4009u, /* Base address: 0x400F4000 Count: 9 */
			0x400F4104u, /* Base address: 0x400F4100 Count: 4 */
			0x400F4205u, /* Base address: 0x400F4200 Count: 5 */
			0x400F4305u, /* Base address: 0x400F4300 Count: 5 */
			0x400F4408u, /* Base address: 0x400F4400 Count: 8 */
			0x400F4505u, /* Base address: 0x400F4500 Count: 5 */
			0x400F4605u, /* Base address: 0x400F4600 Count: 5 */
			0x400F4710u, /* Base address: 0x400F4700 Count: 16 */
			0x400F6002u, /* Base address: 0x400F6000 Count: 2 */
		};

		static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
			{0x81u, 0x0Du},
			{0x85u, 0x7Fu},
			{0x02u, 0x01u},
			{0x0Du, 0x01u},
			{0x0Fu, 0x02u},
			{0x13u, 0x02u},
			{0x16u, 0x01u},
			{0x17u, 0x01u},
			{0x30u, 0x01u},
			{0x31u, 0x03u},
			{0x3Au, 0x30u},
			{0x3Eu, 0x01u},
			{0x3Fu, 0x01u},
			{0x52u, 0x80u},
			{0x58u, 0x04u},
			{0x59u, 0x04u},
			{0x5Bu, 0x09u},
			{0x5Cu, 0x08u},
			{0x5Du, 0x90u},
			{0x5Fu, 0x01u},
			{0x82u, 0x01u},
			{0x83u, 0x3Fu},
			{0x89u, 0x01u},
			{0x8Au, 0x04u},
			{0x8Bu, 0xC0u},
			{0x8Cu, 0x04u},
			{0x8Du, 0xFFu},
			{0x8Eu, 0x08u},
			{0x91u, 0x04u},
			{0x92u, 0x10u},
			{0x93u, 0xC0u},
			{0x95u, 0x02u},
			{0x96u, 0x01u},
			{0x97u, 0xC0u},
			{0x98u, 0x08u},
			{0x99u, 0x40u},
			{0x9Au, 0x04u},
			{0x9Bu, 0x80u},
			{0x9Cu, 0x20u},
			{0x9Fu, 0x7Fu},
			{0xA1u, 0x08u},
			{0xA2u, 0x10u},
			{0xA3u, 0xC0u},
			{0xA5u, 0x10u},
			{0xA7u, 0xC0u},
			{0xA9u, 0xC0u},
			{0xAAu, 0x02u},
			{0xABu, 0x20u},
			{0xAEu, 0x03u},
			{0xAFu, 0x80u},
			{0xB0u, 0x03u},
			{0xB2u, 0x0Cu},
			{0xB4u, 0x10u},
			{0xB6u, 0x20u},
			{0xB7u, 0xFFu},
			{0xBEu, 0x10u},
			{0xC0u, 0x45u},
			{0xC5u, 0xECu},
			{0xC6u, 0x02u},
			{0xC8u, 0x07u},
			{0xC9u, 0xFFu},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCFu, 0x01u},
			{0xD0u, 0x18u},
			{0xD4u, 0x01u},
			{0xD8u, 0x04u},
			{0xD9u, 0x04u},
			{0xDAu, 0x08u},
			{0xDBu, 0x08u},
			{0xDCu, 0x88u},
			{0xDDu, 0x99u},
			{0xDFu, 0x31u},
			{0xE0u, 0x40u},
			{0xE2u, 0x40u},
			{0xE4u, 0x40u},
			{0xE5u, 0x40u},
			{0xE6u, 0x80u},
			{0xE8u, 0x80u},
			{0xEAu, 0x80u},
			{0xECu, 0x80u},
			{0xEEu, 0x80u},
			{0x01u, 0x02u},
			{0x04u, 0x60u},
			{0x05u, 0x02u},
			{0x09u, 0x08u},
			{0x0Du, 0x2Au},
			{0x0Fu, 0x02u},
			{0x14u, 0x01u},
			{0x16u, 0x80u},
			{0x17u, 0x10u},
			{0x1Au, 0x88u},
			{0x1Du, 0x08u},
			{0x1Eu, 0x40u},
			{0x1Fu, 0x50u},
			{0x22u, 0x02u},
			{0x26u, 0x80u},
			{0x27u, 0x02u},
			{0x2Cu, 0x81u},
			{0x2Fu, 0x24u},
			{0x30u, 0x02u},
			{0x32u, 0x08u},
			{0x34u, 0x20u},
			{0x35u, 0x04u},
			{0x36u, 0x80u},
			{0x37u, 0x01u},
			{0x39u, 0x40u},
			{0x3Eu, 0x60u},
			{0x3Fu, 0x02u},
			{0x45u, 0x02u},
			{0x4Du, 0x04u},
			{0x4Fu, 0x81u},
			{0x56u, 0x80u},
			{0x57u, 0x40u},
			{0x5Cu, 0x80u},
			{0x5Du, 0x04u},
			{0x5Fu, 0x01u},
			{0x6Du, 0x40u},
			{0x71u, 0x02u},
			{0x7Au, 0x06u},
			{0x7Du, 0x01u},
			{0x7Fu, 0x01u},
			{0x82u, 0x40u},
			{0x83u, 0x10u},
			{0x86u, 0x04u},
			{0x88u, 0x02u},
			{0xC0u, 0xD8u},
			{0xC2u, 0xF4u},
			{0xC4u, 0xD0u},
			{0xCAu, 0xF0u},
			{0xCCu, 0xF3u},
			{0xCEu, 0xB8u},
			{0xD0u, 0x80u},
			{0xD2u, 0x10u},
			{0xD6u, 0xD0u},
			{0xDEu, 0x9Cu},
			{0x35u, 0x01u},
			{0x3Au, 0x03u},
			{0x3Fu, 0x10u},
			{0x56u, 0x08u},
			{0x58u, 0x04u},
			{0x59u, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Cu, 0x08u},
			{0x5Du, 0x90u},
			{0x5Fu, 0x11u},
			{0x02u, 0x04u},
			{0x03u, 0x60u},
			{0x09u, 0x08u},
			{0x0Au, 0x46u},
			{0x10u, 0x04u},
			{0x11u, 0x02u},
			{0x12u, 0x21u},
			{0x18u, 0x80u},
			{0x19u, 0x21u},
			{0x1Au, 0x44u},
			{0x1Eu, 0x80u},
			{0x21u, 0x10u},
			{0x22u, 0x20u},
			{0x23u, 0x10u},
			{0x25u, 0x10u},
			{0x28u, 0x04u},
			{0x29u, 0x05u},
			{0x2Au, 0xA4u},
			{0x30u, 0x80u},
			{0x31u, 0x08u},
			{0x32u, 0x01u},
			{0x33u, 0x60u},
			{0x38u, 0x08u},
			{0x39u, 0x20u},
			{0x3Au, 0x01u},
			{0x3Bu, 0x88u},
			{0x40u, 0x40u},
			{0x41u, 0x62u},
			{0x42u, 0x10u},
			{0x44u, 0x80u},
			{0x46u, 0x40u},
			{0x49u, 0x02u},
			{0x4Au, 0x0Au},
			{0x53u, 0x10u},
			{0x5Au, 0xA0u},
			{0x5Cu, 0x80u},
			{0x5Eu, 0x08u},
			{0x5Fu, 0x14u},
			{0x67u, 0x06u},
			{0x69u, 0x54u},
			{0x79u, 0x01u},
			{0x7Au, 0x40u},
			{0x7Eu, 0x14u},
			{0x7Fu, 0x10u},
			{0x91u, 0x80u},
			{0x92u, 0x88u},
			{0x93u, 0x80u},
			{0x94u, 0x60u},
			{0x95u, 0x01u},
			{0x97u, 0x0Au},
			{0x9Bu, 0x02u},
			{0x9Cu, 0x21u},
			{0x9Du, 0x06u},
			{0x9Eu, 0x20u},
			{0x9Fu, 0x01u},
			{0xA4u, 0x80u},
			{0xA5u, 0x20u},
			{0xA6u, 0x04u},
			{0xA7u, 0x21u},
			{0xACu, 0x80u},
			{0xAFu, 0x10u},
			{0xB5u, 0x48u},
			{0xB6u, 0x08u},
			{0xC0u, 0x0Eu},
			{0xC2u, 0x0Fu},
			{0xC4u, 0x0Fu},
			{0xCAu, 0x0Fu},
			{0xCCu, 0x0Fu},
			{0xCEu, 0x0Fu},
			{0xD0u, 0x0Fu},
			{0xD2u, 0x0Cu},
			{0xD6u, 0x3Cu},
			{0xD8u, 0x30u},
			{0xDEu, 0x21u},
			{0x00u, 0x80u},
			{0x02u, 0x04u},
			{0x04u, 0x80u},
			{0x06u, 0x10u},
			{0x07u, 0x02u},
			{0x08u, 0x80u},
			{0x0Au, 0x02u},
			{0x0Eu, 0x7Fu},
			{0x0Fu, 0x04u},
			{0x10u, 0xFFu},
			{0x11u, 0x01u},
			{0x14u, 0x80u},
			{0x16u, 0x01u},
			{0x17u, 0x01u},
			{0x19u, 0x01u},
			{0x1Eu, 0x7Fu},
			{0x20u, 0x80u},
			{0x22u, 0x08u},
			{0x26u, 0x80u},
			{0x28u, 0x80u},
			{0x2Au, 0x20u},
			{0x2Cu, 0x80u},
			{0x2Eu, 0x40u},
			{0x31u, 0x01u},
			{0x35u, 0x04u},
			{0x36u, 0xFFu},
			{0x37u, 0x02u},
			{0x56u, 0x08u},
			{0x58u, 0x04u},
			{0x59u, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Cu, 0x88u},
			{0x5Du, 0x80u},
			{0x5Fu, 0x31u},
			{0x83u, 0x8Bu},
			{0x85u, 0x9Bu},
			{0x87u, 0x24u},
			{0x89u, 0x8Au},
			{0x8Bu, 0x01u},
			{0x8Du, 0x0Bu},
			{0x93u, 0xFFu},
			{0x95u, 0x81u},
			{0x97u, 0x02u},
			{0x99u, 0x83u},
			{0x9Bu, 0x44u},
			{0xA5u, 0x01u},
			{0xA7u, 0x8Au},
			{0xADu, 0xA7u},
			{0xAFu, 0x10u},
			{0xB1u, 0x84u},
			{0xB3u, 0x07u},
			{0xB5u, 0x78u},
			{0xB9u, 0x20u},
			{0xD6u, 0x02u},
			{0xD7u, 0x20u},
			{0xD9u, 0x04u},
			{0xDBu, 0x04u},
			{0xDCu, 0x80u},
			{0xDDu, 0x80u},
			{0xDFu, 0x31u},
			{0x01u, 0x82u},
			{0x02u, 0x04u},
			{0x03u, 0x20u},
			{0x08u, 0x04u},
			{0x0Bu, 0x41u},
			{0x11u, 0x92u},
			{0x12u, 0x10u},
			{0x19u, 0x80u},
			{0x1Au, 0x02u},
			{0x22u, 0x52u},
			{0x24u, 0x20u},
			{0x25u, 0x26u},
			{0x26u, 0x28u},
			{0x29u, 0x02u},
			{0x2Au, 0x01u},
			{0x2Fu, 0x22u},
			{0x31u, 0x22u},
			{0x32u, 0x08u},
			{0x33u, 0x02u},
			{0x36u, 0x20u},
			{0x37u, 0x0Au},
			{0x39u, 0x44u},
			{0x3Cu, 0x80u},
			{0x3Du, 0x1Au},
			{0x5Au, 0xA0u},
			{0x60u, 0x04u},
			{0x62u, 0x10u},
			{0x63u, 0x03u},
			{0x67u, 0x01u},
			{0x6Cu, 0x05u},
			{0x6Du, 0x92u},
			{0x6Eu, 0x02u},
			{0x6Fu, 0x60u},
			{0x74u, 0x40u},
			{0x76u, 0x25u},
			{0x77u, 0x19u},
			{0x79u, 0x01u},
			{0x7Au, 0x40u},
			{0x7Du, 0x01u},
			{0x8Cu, 0x21u},
			{0x90u, 0x80u},
			{0x91u, 0x64u},
			{0x94u, 0x28u},
			{0x96u, 0x12u},
			{0x98u, 0x04u},
			{0x99u, 0x20u},
			{0x9Bu, 0x02u},
			{0x9Cu, 0x21u},
			{0x9Eu, 0x25u},
			{0x9Fu, 0x41u},
			{0xA2u, 0xC0u},
			{0xA5u, 0x2Au},
			{0xA6u, 0x0Au},
			{0xA7u, 0x21u},
			{0xADu, 0x10u},
			{0xAEu, 0xC0u},
			{0xAFu, 0x10u},
			{0xB2u, 0x84u},
			{0xB4u, 0x04u},
			{0xB5u, 0x11u},
			{0xC0u, 0x0Fu},
			{0xC2u, 0x0Du},
			{0xC4u, 0x0Fu},
			{0xCAu, 0x50u},
			{0xCCu, 0xE7u},
			{0xCEu, 0xFAu},
			{0xD6u, 0x0Cu},
			{0xD8u, 0x1Cu},
			{0xDEu, 0x81u},
			{0xE2u, 0x02u},
			{0xE8u, 0x08u},
			{0xECu, 0x40u},
			{0x00u, 0x20u},
			{0x02u, 0x1Fu},
			{0x04u, 0x08u},
			{0x06u, 0x01u},
			{0x08u, 0x01u},
			{0x0Au, 0x02u},
			{0x0Cu, 0x02u},
			{0x0Eu, 0x1Cu},
			{0x12u, 0x20u},
			{0x1Cu, 0x04u},
			{0x1Eu, 0x01u},
			{0x22u, 0x1Fu},
			{0x28u, 0x10u},
			{0x2Au, 0x01u},
			{0x30u, 0x3Fu},
			{0x56u, 0x08u},
			{0x58u, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Cu, 0x08u},
			{0x5Du, 0x80u},
			{0x5Fu, 0x31u},
			{0x00u, 0x80u},
			{0x01u, 0x08u},
			{0x02u, 0x80u},
			{0x03u, 0x28u},
			{0x04u, 0x02u},
			{0x05u, 0x08u},
			{0x06u, 0x04u},
			{0x07u, 0x01u},
			{0x08u, 0x04u},
			{0x09u, 0x41u},
			{0x0Du, 0x02u},
			{0x0Eu, 0x02u},
			{0x10u, 0x02u},
			{0x12u, 0x04u},
			{0x14u, 0x04u},
			{0x15u, 0x12u},
			{0x1Au, 0x50u},
			{0x1Bu, 0x08u},
			{0x1Du, 0x02u},
			{0x20u, 0x08u},
			{0x21u, 0x88u},
			{0x22u, 0x01u},
			{0x23u, 0x40u},
			{0x29u, 0xA4u},
			{0x31u, 0x08u},
			{0x32u, 0x01u},
			{0x33u, 0x10u},
			{0x38u, 0x84u},
			{0x39u, 0x20u},
			{0x3Au, 0x80u},
			{0x41u, 0x90u},
			{0x42u, 0x80u},
			{0x44u, 0x10u},
			{0x46u, 0x10u},
			{0x49u, 0x24u},
			{0x51u, 0x01u},
			{0x52u, 0x10u},
			{0x58u, 0x90u},
			{0x5Eu, 0x40u},
			{0x64u, 0x01u},
			{0x79u, 0x01u},
			{0x7Du, 0x01u},
			{0x80u, 0x03u},
			{0x81u, 0x02u},
			{0x84u, 0x84u},
			{0x8Au, 0x10u},
			{0x90u, 0x86u},
			{0x91u, 0x31u},
			{0x96u, 0x10u},
			{0x97u, 0x40u},
			{0x99u, 0xA0u},
			{0x9Au, 0x04u},
			{0x9Bu, 0x11u},
			{0x9Cu, 0x01u},
			{0xA0u, 0x40u},
			{0xA1u, 0x41u},
			{0xA2u, 0x81u},
			{0xA3u, 0x20u},
			{0xA5u, 0x08u},
			{0xC0u, 0xFFu},
			{0xC2u, 0x9Du},
			{0xC4u, 0x53u},
			{0xCAu, 0x0Eu},
			{0xCCu, 0x07u},
			{0xCEu, 0x0Eu},
			{0xD0u, 0x0Du},
			{0xD2u, 0x04u},
			{0xD6u, 0x1Cu},
			{0xD8u, 0x10u},
			{0xDEu, 0x81u},
			{0xE0u, 0x01u},
			{0xE2u, 0x04u},
			{0xE4u, 0x04u},
			{0x67u, 0x08u},
			{0x69u, 0x02u},
			{0x6Fu, 0x01u},
			{0x77u, 0x02u},
			{0x87u, 0x04u},
			{0xD8u, 0x80u},
			{0xDAu, 0x80u},
			{0xDCu, 0x60u},
			{0xE6u, 0x10u},
			{0x29u, 0x01u},
			{0x95u, 0x02u},
			{0x9Fu, 0x03u},
			{0xCAu, 0x40u},
			{0x87u, 0x01u},
			{0x95u, 0x02u},
			{0x9Fu, 0x03u},
			{0xB5u, 0x01u},
			{0xE2u, 0x40u},
			{0x13u, 0x02u},
			{0x15u, 0x02u},
			{0x95u, 0x02u},
			{0x9Fu, 0x02u},
			{0xC4u, 0x30u},
			{0x26u, 0x08u},
			{0x29u, 0x80u},
			{0x54u, 0x20u},
			{0x8Au, 0x08u},
			{0xC8u, 0x01u},
			{0xCAu, 0x08u},
			{0xD4u, 0x02u},
			{0xE6u, 0x01u},
			{0x40u, 0x04u},
			{0xA4u, 0x20u},
			{0xB5u, 0x80u},
			{0xD0u, 0x08u},
			{0xEEu, 0x02u},
			{0x86u, 0x04u},
			{0x93u, 0x80u},
			{0xAFu, 0x40u},
			{0xB4u, 0x24u},
			{0xECu, 0x0Au},
			{0x0Cu, 0x80u},
			{0x2Eu, 0x04u},
			{0x5Bu, 0x04u},
			{0x67u, 0x82u},
			{0x6Bu, 0x01u},
			{0x80u, 0x80u},
			{0x83u, 0x05u},
			{0x8Bu, 0x01u},
			{0x93u, 0x80u},
			{0x96u, 0x04u},
			{0xC2u, 0x01u},
			{0xCAu, 0x01u},
			{0xD6u, 0x03u},
			{0xD8u, 0x01u},
			{0xDAu, 0x02u},
			{0xE4u, 0x04u},
			{0x02u, 0x01u},
			{0x11u, 0x01u},
		};



		CYPACKED typedef struct {
			void CYFAR *address;
			uint16 size;
		} CYPACKED_ATTR cfg_memset_t;


		CYPACKED typedef struct {
			void CYFAR *dest;
			const void CYCODE *src;
			uint16 size;
		} CYPACKED_ATTR cfg_memcpy_t;

		static const cfg_memset_t CYCODE cfg_memset_list[] = {
			/* address, size */
			{(void CYFAR *)(CYDEV_UDB_P0_U0_BASE), 640u},
			{(void CYFAR *)(CYDEV_UDB_P1_ROUTE_BASE), 896u},
			{(void CYFAR *)(CYDEV_UDB_P3_ROUTE_BASE), 256u},
			{(void CYFAR *)(CYDEV_UDB_DSI0_BASE), 2048u},
		};

		/* UDB_0_2_0_CONFIG Address: CYDEV_UDB_P1_U1_BASE Size (bytes): 128 */
		static const uint8 CYCODE BS_UDB_0_2_0_CONFIG_VAL[] = {
			0x63u, 0x06u, 0x8Cu, 0x08u, 0x63u, 0x00u, 0x8Cu, 0x0Eu, 0xE3u, 0x40u, 0x04u, 0x10u, 0x00u, 0x01u, 0x00u, 0x00u, 
			0xEFu, 0x00u, 0x00u, 0xFEu, 0x00u, 0x00u, 0xCEu, 0x0Cu, 0xEFu, 0x06u, 0x00u, 0x08u, 0x10u, 0x06u, 0x00u, 0x08u, 
			0x60u, 0x80u, 0x8Fu, 0x20u, 0xEFu, 0x06u, 0x00u, 0x00u, 0x21u, 0x03u, 0x00u, 0x00u, 0x63u, 0x3Eu, 0x88u, 0xC0u, 
			0x1Fu, 0x00u, 0x00u, 0x01u, 0xF0u, 0xFEu, 0x00u, 0x00u, 0x00u, 0x20u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x42u, 0x06u, 0x51u, 0x00u, 0x03u, 0xC0u, 0xE0u, 0x00u, 0x0Au, 0xFFu, 0xFFu, 0xFFu, 0x02u, 0x20u, 0x00u, 0x05u, 
			0x04u, 0x12u, 0x07u, 0x00u, 0x01u, 0x00u, 0x00u, 0x00u, 0x04u, 0x04u, 0x04u, 0x04u, 0x88u, 0x88u, 0x00u, 0x31u, 
			0x00u, 0x00u, 0x40u, 0x01u, 0x40u, 0x01u, 0x50u, 0x01u, 0x40u, 0x01u, 0x70u, 0x10u, 0x40u, 0x10u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

		/* UDB_0_0_0_CONFIG Address: CYDEV_UDB_P3_U1_BASE Size (bytes): 128 */
		static const uint8 CYCODE BS_UDB_0_0_0_CONFIG_VAL[] = {
			0x00u, 0x00u, 0x40u, 0x00u, 0x08u, 0x00u, 0x01u, 0x03u, 0x00u, 0x22u, 0x20u, 0x0Cu, 0x02u, 0x0Cu, 0x1Cu, 0x22u, 
			0x04u, 0x00u, 0x01u, 0x01u, 0x10u, 0x00u, 0x01u, 0x10u, 0x00u, 0x08u, 0x00u, 0x00u, 0x1Fu, 0x00u, 0x20u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x22u, 0x01u, 0x22u, 0x02u, 0x0Cu, 0x00u, 0x00u, 0x1Fu, 0x18u, 
			0x40u, 0x03u, 0x3Fu, 0x18u, 0x00u, 0x20u, 0x00u, 0x04u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x10u, 
			0x24u, 0x06u, 0x00u, 0x01u, 0x00u, 0x0Cu, 0xE0u, 0xB0u, 0x29u, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x04u, 0x10u, 0x00u, 0x00u, 0x01u, 0x00u, 0x00u, 0x00u, 0x04u, 0x04u, 0x04u, 0x04u, 0x88u, 0x88u, 0x00u, 0x31u, 
			0x00u, 0x00u, 0xC0u, 0x00u, 0x40u, 0x01u, 0x80u, 0x00u, 0x30u, 0x00u, 0x40u, 0x10u, 0x10u, 0x00u, 0x40u, 0xA8u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

		static const cfg_memcpy_t CYCODE cfg_memcpy_list [] = {
			/* dest, src, size */
			{(void CYFAR *)(CYDEV_UDB_P1_U1_BASE), BS_UDB_0_2_0_CONFIG_VAL, 128u},
			{(void CYFAR *)(CYDEV_UDB_P3_U1_BASE), BS_UDB_0_0_0_CONFIG_VAL, 128u},
		};

		uint8 CYDATA i;

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t CYCODE * CYDATA ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, (size_t)(uint32)(ms->size));
		}

		/* Copy device configuration data into registers */
		for (i = 0u; i < (sizeof(cfg_memcpy_list)/sizeof(cfg_memcpy_list[0])); i++)
		{
			const cfg_memcpy_t CYCODE * CYDATA mc = &cfg_memcpy_list[i];
			void * CYDATA destPtr = mc->dest;
			const void CYCODE * CYDATA srcPtr = mc->src;
			uint16 CYDATA numBytes = mc->size;
			CYCONFIGCPYCODE(destPtr, srcPtr, numBytes);
		}

		cfg_write_bytes32(cy_cfg_addr_table, cy_cfg_data_table);

		/* HSIOM Starting address: CYDEV_HSIOM_BASE */
		CY_SET_REG32((void *)(CYDEV_HSIOM_BASE), 0x00000670u);
		CY_SET_REG32((void *)(CYREG_HSIOM_PORT_SEL3), 0x0333EE00u);
		CY_SET_REG32((void *)(CYREG_HSIOM_PORT_SEL4), 0x000000EEu);
		CY_SET_REG32((void *)(CYREG_HSIOM_PORT_SEL5), 0x30000000u);
		CY_SET_REG32((void *)(CYREG_HSIOM_PORT_SEL8), 0x00000099u);
		CY_SET_REG32((void *)(CYREG_HSIOM_PORT_SEL11), 0x08080800u);

		/* UDB_PA_0 Starting address: CYDEV_UDB_PA0_BASE */
		CY_SET_REG32((void *)(CYDEV_UDB_PA0_BASE), 0x00990000u);
		CY_SET_REG32((void *)(CYREG_UDB_PA0_CFG8), 0xC0000000u);

		/* UDB_PA_2 Starting address: CYDEV_UDB_PA2_BASE */
		CY_SET_REG32((void *)(CYDEV_UDB_PA2_BASE), 0x00990000u);

		/* UDB_PA_4 Starting address: CYDEV_UDB_PA4_BASE */
		CY_SET_REG32((void *)(CYDEV_UDB_PA4_BASE), 0x00990000u);
		CY_SET_REG32((void *)(CYREG_UDB_PA4_CFG4), 0x00800000u);
		CY_SET_REG32((void *)(CYREG_UDB_PA4_CFG8), 0x00140000u);

		/* UDB_PA_5 Starting address: CYDEV_UDB_PA5_BASE */
		CY_SET_REG32((void *)(CYDEV_UDB_PA5_BASE), 0x00990000u);

		/* UDB_PA_6 Starting address: CYDEV_UDB_PA6_BASE */
		CY_SET_REG32((void *)(CYDEV_UDB_PA6_BASE), 0x00990000u);

		/* UDB_PA_7 Starting address: CYDEV_UDB_PA7_BASE */
		CY_SET_REG32((void *)(CYDEV_UDB_PA7_BASE), 0x00990000u);
		CY_SET_REG32((void *)(CYREG_UDB_PA7_CFG8), 0x8D000000u);

		/* TCPWM_CNT0 Starting address: CYDEV_TCPWM_CNT0_TR_CTRL0 */
		CY_SET_REG32((void *)(CYREG_TCPWM_CNT0_TR_CTRL0), 0x00000090u);

		/* INT_SELECT Starting address: CYDEV_CPUSS_INT_SEL */
		CY_SET_REG32((void *)(CYREG_CPUSS_INT_SEL), 0x0000000Eu);

		/* TR_GROUP0 Starting address: CYDEV_PERI_TR_GROUP0_TR_OUT_CTL0 */
		CY_SET_REG32((void *)(CYREG_PERI_TR_GROUP0_TR_OUT_CTL0), 0x00000022u);
		CY_SET_REG32((void *)(CYREG_PERI_TR_GROUP0_TR_OUT_CTL2), 0x00000019u);

		/* INT_CONFIG Starting address: CYDEV_UDB_INT_CFG */
		CY_SET_REG32((void *)(CYREG_UDB_INT_CFG), 0x0000000Cu);

		/* Enable UDB array and digital routing */
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDB_WAIT_CFG, (uint8)((CY_GET_XTND_REG8((void *)CYREG_UDB_UDB_WAIT_CFG) & 0xC3u) | 0x14u));
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDB_BANK_CTL, (uint8)(CY_GET_XTND_REG8((void *)CYREG_UDB_UDB_BANK_CTL) | 0x16u));
	}

	/* Perform second pass device configuration. These items must be configured in specific order after the regular configuration is done. */
	/* IOPINS0_0 Starting address: CYDEV_GPIO_PRT0_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT0_BASE), 0x00000006u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT0_PC), 0x00000406u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT0_INTR_CFG), 0x000000C0u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT0_PC2), 0x00000006u);

	/* IOPINS0_11 Starting address: CYDEV_GPIO_PRT11_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT11_BASE), 0x00000054u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT11_PC), 0x00186180u);

	/* IOPINS0_13 Starting address: CYDEV_GPIO_PRT13_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT13_BASE), 0x00000003u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT13_INTR_CFG), 0x00000002u);

	/* IOPINS0_2 Starting address: CYDEV_GPIO_PRT2_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT2_BASE), 0x0000000Du);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT2_PC), 0x00000D86u);

	/* IOPINS0_3 Starting address: CYDEV_GPIO_PRT3_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT3_BASE), 0x00000070u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT3_PC), 0x003B6D80u);

	/* IOPINS0_4 Starting address: CYDEV_GPIO_PRT4_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT4_BASE), 0x00000003u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT4_PC), 0x00000024u);

	/* IOPINS0_5 Starting address: CYDEV_GPIO_PRT5_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT5_BASE), 0x00000080u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT5_PC), 0x00C00000u);

	/* IOPINS0_8 Starting address: CYDEV_GPIO_PRT8_BASE */
	CY_SET_REG32((void *)(CYDEV_GPIO_PRT8_BASE), 0x00000002u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT8_PC), 0x00000031u);
	CY_SET_REG32((void *)(CYREG_GPIO_PRT8_PC2), 0x00000002u);


	/* Setup clocks based on selections from Clock DWR */
	ClockSetup();

	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

}
