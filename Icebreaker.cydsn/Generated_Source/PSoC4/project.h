/*******************************************************************************
* File Name: project.h
* 
* PSoC Creator  4.2
*
* Description:
* It contains references to all generated header files and should not be modified.
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cyfitter_cfg.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cydisabledsheets.h"
#include "I2S.h"
#include "I2S_PVT.h"
#include "Codec_MCLK.h"
#include "Codec_MCLK_aliases.h"
#include "Async_Feedback_Counter.h"
#include "Codec_DACDAT.h"
#include "Codec_DACDAT_aliases.h"
#include "Codec_BCLK.h"
#include "Codec_BCLK_aliases.h"
#include "Codec_LRC.h"
#include "Codec_LRC_aliases.h"
#include "ByteCounter_Tx.h"
#include "TxDMA.h"
#include "isr_TxDMADone.h"
#include "ByteCounter_Rx.h"
#include "isr_RxDMADone.h"
#include "Clk_Counter.h"
#include "UART.h"
#include "UART_SPI_UART.h"
#include "UART_PINS.h"
#include "UART_SPI_UART_PVT.h"
#include "UART_PVT.h"
#include "UART_BOOT.h"
#include "CodecI2CM.h"
#include "CodecI2CM_I2C.h"
#include "CodecI2CM_PINS.h"
#include "CodecI2CM_I2C_PVT.h"
#include "CodecI2CM_PVT.h"
#include "CodecI2CM_BOOT.h"
#include "AudioClkSel.h"
#include "LED_BLUE.h"
#include "LED_BLUE_aliases.h"
#include "ADC.h"
#include "POTR.h"
#include "POTR_aliases.h"
#include "DMA.h"
#include "POTL.h"
#include "POTL_aliases.h"
#include "RSTn.h"
#include "RSTn_aliases.h"
#include "USBUART.h"
#include "USBUART_audio.h"
#include "USBUART_cdc.h"
#include "USBUART_hid.h"
#include "USBUART_midi.h"
#include "USBUART_pvt.h"
#include "USBUART_cydmac.h"
#include "USBUART_msc.h"
#include "BTN.h"
#include "BTN_aliases.h"
#include "isr_Button.h"
#include "LED_RED_PWM.h"
#include "LED_RED_PWM_ISR.h"
#include "Clock_1.h"
#include "LED_RED.h"
#include "LED_RED_aliases.h"
#include "LED_BLUE_PWM.h"
#include "LED_BLUE_PWM_ISR.h"
#include "LED_GREEN_PWM.h"
#include "LED_GREEN_PWM_ISR.h"
#include "LED_GREEN.h"
#include "LED_GREEN_aliases.h"
#include "SPI.h"
#include "SPI_SPI_UART.h"
#include "SPI_PINS.h"
#include "SPI_SPI_UART_PVT.h"
#include "SPI_PVT.h"
#include "SPI_BOOT.h"
#include "RF_MCLK.h"
#include "RF_MCLK_aliases.h"
#include "EHIF_IRQ.h"
#include "EHIF_IRQ_aliases.h"
#include "EHIF_IRQ_ISR.h"
#include "UART_SCBCLK.h"
#include "UART_tx.h"
#include "UART_tx_aliases.h"
#include "UART_rx.h"
#include "UART_rx_aliases.h"
#include "CodecI2CM_SCBCLK.h"
#include "CodecI2CM_sda.h"
#include "CodecI2CM_sda_aliases.h"
#include "CodecI2CM_scl.h"
#include "CodecI2CM_scl_aliases.h"
#include "CodecI2CM_SCB_IRQ.h"
#include "ADC_IRQ.h"
#include "ADC_intClock.h"
#include "USBUART_Dp.h"
#include "USBUART_Dp_aliases.h"
#include "USBUART_Dm.h"
#include "USBUART_Dm_aliases.h"
#include "SPI_SCBCLK.h"
#include "SPI_sclk_m.h"
#include "SPI_sclk_m_aliases.h"
#include "SPI_miso_m.h"
#include "SPI_miso_m_aliases.h"
#include "SPI_mosi_m.h"
#include "SPI_mosi_m_aliases.h"
#include "SPI_ss0_m.h"
#include "SPI_ss0_m_aliases.h"
#include "SPI_SCB_IRQ.h"
#include "cy_em_eeprom.h"
#include "core_cm0_psoc4.h"
#include "CyFlash.h"
#include "CyLib.h"
#include "cyPm.h"
#include "cytypes.h"
#include "cypins.h"
#include "CyDMA.h"
#include "CyLFClk.h"

/*[]*/

