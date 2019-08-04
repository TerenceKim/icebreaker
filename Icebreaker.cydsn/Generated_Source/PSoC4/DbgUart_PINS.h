/***************************************************************************//**
* \file DbgUart_PINS.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
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

#if !defined(CY_SCB_PINS_DbgUart_H)
#define CY_SCB_PINS_DbgUart_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define DbgUart_REMOVE_RX_WAKE_SCL_MOSI_PIN  (1u)
#define DbgUart_REMOVE_RX_SCL_MOSI_PIN      (1u)
#define DbgUart_REMOVE_TX_SDA_MISO_PIN      (1u)
#define DbgUart_REMOVE_CTS_SCLK_PIN      (1u)
#define DbgUart_REMOVE_RTS_SS0_PIN      (1u)
#define DbgUart_REMOVE_SS1_PIN                 (1u)
#define DbgUart_REMOVE_SS2_PIN                 (1u)
#define DbgUart_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define DbgUart_REMOVE_I2C_PINS                (1u)
#define DbgUart_REMOVE_SPI_MASTER_PINS         (1u)
#define DbgUart_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define DbgUart_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define DbgUart_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define DbgUart_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define DbgUart_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define DbgUart_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define DbgUart_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define DbgUart_REMOVE_SPI_SLAVE_PINS          (1u)
#define DbgUart_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define DbgUart_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define DbgUart_REMOVE_UART_TX_PIN             (0u)
#define DbgUart_REMOVE_UART_RX_TX_PIN          (1u)
#define DbgUart_REMOVE_UART_RX_PIN             (0u)
#define DbgUart_REMOVE_UART_RX_WAKE_PIN        (1u)
#define DbgUart_REMOVE_UART_RTS_PIN            (1u)
#define DbgUart_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define DbgUart_RX_WAKE_SCL_MOSI_PIN (0u == DbgUart_REMOVE_RX_WAKE_SCL_MOSI_PIN)
#define DbgUart_RX_SCL_MOSI_PIN     (0u == DbgUart_REMOVE_RX_SCL_MOSI_PIN)
#define DbgUart_TX_SDA_MISO_PIN     (0u == DbgUart_REMOVE_TX_SDA_MISO_PIN)
#define DbgUart_CTS_SCLK_PIN     (0u == DbgUart_REMOVE_CTS_SCLK_PIN)
#define DbgUart_RTS_SS0_PIN     (0u == DbgUart_REMOVE_RTS_SS0_PIN)
#define DbgUart_SS1_PIN                (0u == DbgUart_REMOVE_SS1_PIN)
#define DbgUart_SS2_PIN                (0u == DbgUart_REMOVE_SS2_PIN)
#define DbgUart_SS3_PIN                (0u == DbgUart_REMOVE_SS3_PIN)

/* Mode defined pins */
#define DbgUart_I2C_PINS               (0u == DbgUart_REMOVE_I2C_PINS)
#define DbgUart_SPI_MASTER_PINS        (0u == DbgUart_REMOVE_SPI_MASTER_PINS)
#define DbgUart_SPI_MASTER_SCLK_PIN    (0u == DbgUart_REMOVE_SPI_MASTER_SCLK_PIN)
#define DbgUart_SPI_MASTER_MOSI_PIN    (0u == DbgUart_REMOVE_SPI_MASTER_MOSI_PIN)
#define DbgUart_SPI_MASTER_MISO_PIN    (0u == DbgUart_REMOVE_SPI_MASTER_MISO_PIN)
#define DbgUart_SPI_MASTER_SS0_PIN     (0u == DbgUart_REMOVE_SPI_MASTER_SS0_PIN)
#define DbgUart_SPI_MASTER_SS1_PIN     (0u == DbgUart_REMOVE_SPI_MASTER_SS1_PIN)
#define DbgUart_SPI_MASTER_SS2_PIN     (0u == DbgUart_REMOVE_SPI_MASTER_SS2_PIN)
#define DbgUart_SPI_MASTER_SS3_PIN     (0u == DbgUart_REMOVE_SPI_MASTER_SS3_PIN)
#define DbgUart_SPI_SLAVE_PINS         (0u == DbgUart_REMOVE_SPI_SLAVE_PINS)
#define DbgUart_SPI_SLAVE_MOSI_PIN     (0u == DbgUart_REMOVE_SPI_SLAVE_MOSI_PIN)
#define DbgUart_SPI_SLAVE_MISO_PIN     (0u == DbgUart_REMOVE_SPI_SLAVE_MISO_PIN)
#define DbgUart_UART_TX_PIN            (0u == DbgUart_REMOVE_UART_TX_PIN)
#define DbgUart_UART_RX_TX_PIN         (0u == DbgUart_REMOVE_UART_RX_TX_PIN)
#define DbgUart_UART_RX_PIN            (0u == DbgUart_REMOVE_UART_RX_PIN)
#define DbgUart_UART_RX_WAKE_PIN       (0u == DbgUart_REMOVE_UART_RX_WAKE_PIN)
#define DbgUart_UART_RTS_PIN           (0u == DbgUart_REMOVE_UART_RTS_PIN)
#define DbgUart_UART_CTS_PIN           (0u == DbgUart_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (DbgUart_RX_WAKE_SCL_MOSI_PIN)
    #include "DbgUart_uart_rx_wake_i2c_scl_spi_mosi.h"
#endif /* (DbgUart_RX_SCL_MOSI) */

#if (DbgUart_RX_SCL_MOSI_PIN)
    #include "DbgUart_uart_rx_i2c_scl_spi_mosi.h"
#endif /* (DbgUart_RX_SCL_MOSI) */

#if (DbgUart_TX_SDA_MISO_PIN)
    #include "DbgUart_uart_tx_i2c_sda_spi_miso.h"
#endif /* (DbgUart_TX_SDA_MISO) */

#if (DbgUart_CTS_SCLK_PIN)
    #include "DbgUart_uart_cts_spi_sclk.h"
#endif /* (DbgUart_CTS_SCLK) */

#if (DbgUart_RTS_SS0_PIN)
    #include "DbgUart_uart_rts_spi_ss0.h"
#endif /* (DbgUart_RTS_SS0_PIN) */

#if (DbgUart_SS1_PIN)
    #include "DbgUart_spi_ss1.h"
#endif /* (DbgUart_SS1_PIN) */

#if (DbgUart_SS2_PIN)
    #include "DbgUart_spi_ss2.h"
#endif /* (DbgUart_SS2_PIN) */

#if (DbgUart_SS3_PIN)
    #include "DbgUart_spi_ss3.h"
#endif /* (DbgUart_SS3_PIN) */

#if (DbgUart_I2C_PINS)
    #include "DbgUart_scl.h"
    #include "DbgUart_sda.h"
#endif /* (DbgUart_I2C_PINS) */

#if (DbgUart_SPI_MASTER_PINS)
#if (DbgUart_SPI_MASTER_SCLK_PIN)
    #include "DbgUart_sclk_m.h"
#endif /* (DbgUart_SPI_MASTER_SCLK_PIN) */

#if (DbgUart_SPI_MASTER_MOSI_PIN)
    #include "DbgUart_mosi_m.h"
#endif /* (DbgUart_SPI_MASTER_MOSI_PIN) */

#if (DbgUart_SPI_MASTER_MISO_PIN)
    #include "DbgUart_miso_m.h"
#endif /*(DbgUart_SPI_MASTER_MISO_PIN) */
#endif /* (DbgUart_SPI_MASTER_PINS) */

#if (DbgUart_SPI_SLAVE_PINS)
    #include "DbgUart_sclk_s.h"
    #include "DbgUart_ss_s.h"

#if (DbgUart_SPI_SLAVE_MOSI_PIN)
    #include "DbgUart_mosi_s.h"
#endif /* (DbgUart_SPI_SLAVE_MOSI_PIN) */

#if (DbgUart_SPI_SLAVE_MISO_PIN)
    #include "DbgUart_miso_s.h"
#endif /*(DbgUart_SPI_SLAVE_MISO_PIN) */
#endif /* (DbgUart_SPI_SLAVE_PINS) */

#if (DbgUart_SPI_MASTER_SS0_PIN)
    #include "DbgUart_ss0_m.h"
#endif /* (DbgUart_SPI_MASTER_SS0_PIN) */

#if (DbgUart_SPI_MASTER_SS1_PIN)
    #include "DbgUart_ss1_m.h"
#endif /* (DbgUart_SPI_MASTER_SS1_PIN) */

#if (DbgUart_SPI_MASTER_SS2_PIN)
    #include "DbgUart_ss2_m.h"
#endif /* (DbgUart_SPI_MASTER_SS2_PIN) */

#if (DbgUart_SPI_MASTER_SS3_PIN)
    #include "DbgUart_ss3_m.h"
#endif /* (DbgUart_SPI_MASTER_SS3_PIN) */

#if (DbgUart_UART_TX_PIN)
    #include "DbgUart_tx.h"
#endif /* (DbgUart_UART_TX_PIN) */

#if (DbgUart_UART_RX_TX_PIN)
    #include "DbgUart_rx_tx.h"
#endif /* (DbgUart_UART_RX_TX_PIN) */

#if (DbgUart_UART_RX_PIN)
    #include "DbgUart_rx.h"
#endif /* (DbgUart_UART_RX_PIN) */

#if (DbgUart_UART_RX_WAKE_PIN)
    #include "DbgUart_rx_wake.h"
#endif /* (DbgUart_UART_RX_WAKE_PIN) */

#if (DbgUart_UART_RTS_PIN)
    #include "DbgUart_rts.h"
#endif /* (DbgUart_UART_RTS_PIN) */

#if (DbgUart_UART_CTS_PIN)
    #include "DbgUart_cts.h"
#endif /* (DbgUart_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (DbgUart_RX_SCL_MOSI_PIN)
    #define DbgUart_RX_SCL_MOSI_HSIOM_REG   (*(reg32 *) DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    #define DbgUart_RX_SCL_MOSI_HSIOM_PTR   ( (reg32 *) DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    
    #define DbgUart_RX_SCL_MOSI_HSIOM_MASK      (DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define DbgUart_RX_SCL_MOSI_HSIOM_POS       (DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define DbgUart_RX_SCL_MOSI_HSIOM_SEL_GPIO  (DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define DbgUart_RX_SCL_MOSI_HSIOM_SEL_I2C   (DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define DbgUart_RX_SCL_MOSI_HSIOM_SEL_SPI   (DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define DbgUart_RX_SCL_MOSI_HSIOM_SEL_UART  (DbgUart_uart_rx_i2c_scl_spi_mosi__0__HSIOM_UART)
    
#elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG   (*(reg32 *) DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_PTR   ( (reg32 *) DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_MASK      (DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_POS       (DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_SEL_GPIO  (DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C   (DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI   (DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define DbgUart_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART  (DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_UART)    
   
    #define DbgUart_RX_WAKE_SCL_MOSI_INTCFG_REG (*(reg32 *) DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define DbgUart_RX_WAKE_SCL_MOSI_INTCFG_PTR ( (reg32 *) DbgUart_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define DbgUart_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS  (DbgUart_uart_rx_wake_i2c_scl_spi_mosi__SHIFT)
    #define DbgUart_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK ((uint32) DbgUart_INTCFG_TYPE_MASK << \
                                                                           DbgUart_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins DbgUart_RX_SCL_MOSI_PIN or DbgUart_RX_WAKE_SCL_MOSI_PIN present.*/
#endif /* (DbgUart_RX_SCL_MOSI_PIN) */

#if (DbgUart_TX_SDA_MISO_PIN)
    #define DbgUart_TX_SDA_MISO_HSIOM_REG   (*(reg32 *) DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    #define DbgUart_TX_SDA_MISO_HSIOM_PTR   ( (reg32 *) DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    
    #define DbgUart_TX_SDA_MISO_HSIOM_MASK      (DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM_MASK)
    #define DbgUart_TX_SDA_MISO_HSIOM_POS       (DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM_SHIFT)
    #define DbgUart_TX_SDA_MISO_HSIOM_SEL_GPIO  (DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM_GPIO)
    #define DbgUart_TX_SDA_MISO_HSIOM_SEL_I2C   (DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM_I2C)
    #define DbgUart_TX_SDA_MISO_HSIOM_SEL_SPI   (DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM_SPI)
    #define DbgUart_TX_SDA_MISO_HSIOM_SEL_UART  (DbgUart_uart_tx_i2c_sda_spi_miso__0__HSIOM_UART)
#endif /* (DbgUart_TX_SDA_MISO_PIN) */

#if (DbgUart_CTS_SCLK_PIN)
    #define DbgUart_CTS_SCLK_HSIOM_REG   (*(reg32 *) DbgUart_uart_cts_spi_sclk__0__HSIOM)
    #define DbgUart_CTS_SCLK_HSIOM_PTR   ( (reg32 *) DbgUart_uart_cts_spi_sclk__0__HSIOM)
    
    #define DbgUart_CTS_SCLK_HSIOM_MASK      (DbgUart_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define DbgUart_CTS_SCLK_HSIOM_POS       (DbgUart_uart_cts_spi_sclk__0__HSIOM_SHIFT)
    #define DbgUart_CTS_SCLK_HSIOM_SEL_GPIO  (DbgUart_uart_cts_spi_sclk__0__HSIOM_GPIO)
    #define DbgUart_CTS_SCLK_HSIOM_SEL_I2C   (DbgUart_uart_cts_spi_sclk__0__HSIOM_I2C)
    #define DbgUart_CTS_SCLK_HSIOM_SEL_SPI   (DbgUart_uart_cts_spi_sclk__0__HSIOM_SPI)
    #define DbgUart_CTS_SCLK_HSIOM_SEL_UART  (DbgUart_uart_cts_spi_sclk__0__HSIOM_UART)
#endif /* (DbgUart_CTS_SCLK_PIN) */

#if (DbgUart_RTS_SS0_PIN)
    #define DbgUart_RTS_SS0_HSIOM_REG   (*(reg32 *) DbgUart_uart_rts_spi_ss0__0__HSIOM)
    #define DbgUart_RTS_SS0_HSIOM_PTR   ( (reg32 *) DbgUart_uart_rts_spi_ss0__0__HSIOM)
    
    #define DbgUart_RTS_SS0_HSIOM_MASK      (DbgUart_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define DbgUart_RTS_SS0_HSIOM_POS       (DbgUart_uart_rts_spi_ss0__0__HSIOM_SHIFT)
    #define DbgUart_RTS_SS0_HSIOM_SEL_GPIO  (DbgUart_uart_rts_spi_ss0__0__HSIOM_GPIO)
    #define DbgUart_RTS_SS0_HSIOM_SEL_I2C   (DbgUart_uart_rts_spi_ss0__0__HSIOM_I2C)
    #define DbgUart_RTS_SS0_HSIOM_SEL_SPI   (DbgUart_uart_rts_spi_ss0__0__HSIOM_SPI)
#if !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1)
    #define DbgUart_RTS_SS0_HSIOM_SEL_UART  (DbgUart_uart_rts_spi_ss0__0__HSIOM_UART)
#endif /* !(DbgUart_CY_SCBIP_V0 || DbgUart_CY_SCBIP_V1) */
#endif /* (DbgUart_RTS_SS0_PIN) */

#if (DbgUart_SS1_PIN)
    #define DbgUart_SS1_HSIOM_REG  (*(reg32 *) DbgUart_spi_ss1__0__HSIOM)
    #define DbgUart_SS1_HSIOM_PTR  ( (reg32 *) DbgUart_spi_ss1__0__HSIOM)
    
    #define DbgUart_SS1_HSIOM_MASK     (DbgUart_spi_ss1__0__HSIOM_MASK)
    #define DbgUart_SS1_HSIOM_POS      (DbgUart_spi_ss1__0__HSIOM_SHIFT)
    #define DbgUart_SS1_HSIOM_SEL_GPIO (DbgUart_spi_ss1__0__HSIOM_GPIO)
    #define DbgUart_SS1_HSIOM_SEL_I2C  (DbgUart_spi_ss1__0__HSIOM_I2C)
    #define DbgUart_SS1_HSIOM_SEL_SPI  (DbgUart_spi_ss1__0__HSIOM_SPI)
#endif /* (DbgUart_SS1_PIN) */

#if (DbgUart_SS2_PIN)
    #define DbgUart_SS2_HSIOM_REG     (*(reg32 *) DbgUart_spi_ss2__0__HSIOM)
    #define DbgUart_SS2_HSIOM_PTR     ( (reg32 *) DbgUart_spi_ss2__0__HSIOM)
    
    #define DbgUart_SS2_HSIOM_MASK     (DbgUart_spi_ss2__0__HSIOM_MASK)
    #define DbgUart_SS2_HSIOM_POS      (DbgUart_spi_ss2__0__HSIOM_SHIFT)
    #define DbgUart_SS2_HSIOM_SEL_GPIO (DbgUart_spi_ss2__0__HSIOM_GPIO)
    #define DbgUart_SS2_HSIOM_SEL_I2C  (DbgUart_spi_ss2__0__HSIOM_I2C)
    #define DbgUart_SS2_HSIOM_SEL_SPI  (DbgUart_spi_ss2__0__HSIOM_SPI)
#endif /* (DbgUart_SS2_PIN) */

#if (DbgUart_SS3_PIN)
    #define DbgUart_SS3_HSIOM_REG     (*(reg32 *) DbgUart_spi_ss3__0__HSIOM)
    #define DbgUart_SS3_HSIOM_PTR     ( (reg32 *) DbgUart_spi_ss3__0__HSIOM)
    
    #define DbgUart_SS3_HSIOM_MASK     (DbgUart_spi_ss3__0__HSIOM_MASK)
    #define DbgUart_SS3_HSIOM_POS      (DbgUart_spi_ss3__0__HSIOM_SHIFT)
    #define DbgUart_SS3_HSIOM_SEL_GPIO (DbgUart_spi_ss3__0__HSIOM_GPIO)
    #define DbgUart_SS3_HSIOM_SEL_I2C  (DbgUart_spi_ss3__0__HSIOM_I2C)
    #define DbgUart_SS3_HSIOM_SEL_SPI  (DbgUart_spi_ss3__0__HSIOM_SPI)
#endif /* (DbgUart_SS3_PIN) */

#if (DbgUart_I2C_PINS)
    #define DbgUart_SCL_HSIOM_REG  (*(reg32 *) DbgUart_scl__0__HSIOM)
    #define DbgUart_SCL_HSIOM_PTR  ( (reg32 *) DbgUart_scl__0__HSIOM)
    
    #define DbgUart_SCL_HSIOM_MASK     (DbgUart_scl__0__HSIOM_MASK)
    #define DbgUart_SCL_HSIOM_POS      (DbgUart_scl__0__HSIOM_SHIFT)
    #define DbgUart_SCL_HSIOM_SEL_GPIO (DbgUart_sda__0__HSIOM_GPIO)
    #define DbgUart_SCL_HSIOM_SEL_I2C  (DbgUart_sda__0__HSIOM_I2C)
    
    #define DbgUart_SDA_HSIOM_REG  (*(reg32 *) DbgUart_sda__0__HSIOM)
    #define DbgUart_SDA_HSIOM_PTR  ( (reg32 *) DbgUart_sda__0__HSIOM)
    
    #define DbgUart_SDA_HSIOM_MASK     (DbgUart_sda__0__HSIOM_MASK)
    #define DbgUart_SDA_HSIOM_POS      (DbgUart_sda__0__HSIOM_SHIFT)
    #define DbgUart_SDA_HSIOM_SEL_GPIO (DbgUart_sda__0__HSIOM_GPIO)
    #define DbgUart_SDA_HSIOM_SEL_I2C  (DbgUart_sda__0__HSIOM_I2C)
#endif /* (DbgUart_I2C_PINS) */

#if (DbgUart_SPI_SLAVE_PINS)
    #define DbgUart_SCLK_S_HSIOM_REG   (*(reg32 *) DbgUart_sclk_s__0__HSIOM)
    #define DbgUart_SCLK_S_HSIOM_PTR   ( (reg32 *) DbgUart_sclk_s__0__HSIOM)
    
    #define DbgUart_SCLK_S_HSIOM_MASK      (DbgUart_sclk_s__0__HSIOM_MASK)
    #define DbgUart_SCLK_S_HSIOM_POS       (DbgUart_sclk_s__0__HSIOM_SHIFT)
    #define DbgUart_SCLK_S_HSIOM_SEL_GPIO  (DbgUart_sclk_s__0__HSIOM_GPIO)
    #define DbgUart_SCLK_S_HSIOM_SEL_SPI   (DbgUart_sclk_s__0__HSIOM_SPI)
    
    #define DbgUart_SS0_S_HSIOM_REG    (*(reg32 *) DbgUart_ss0_s__0__HSIOM)
    #define DbgUart_SS0_S_HSIOM_PTR    ( (reg32 *) DbgUart_ss0_s__0__HSIOM)
    
    #define DbgUart_SS0_S_HSIOM_MASK       (DbgUart_ss0_s__0__HSIOM_MASK)
    #define DbgUart_SS0_S_HSIOM_POS        (DbgUart_ss0_s__0__HSIOM_SHIFT)
    #define DbgUart_SS0_S_HSIOM_SEL_GPIO   (DbgUart_ss0_s__0__HSIOM_GPIO)  
    #define DbgUart_SS0_S_HSIOM_SEL_SPI    (DbgUart_ss0_s__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_SLAVE_PINS) */

#if (DbgUart_SPI_SLAVE_MOSI_PIN)
    #define DbgUart_MOSI_S_HSIOM_REG   (*(reg32 *) DbgUart_mosi_s__0__HSIOM)
    #define DbgUart_MOSI_S_HSIOM_PTR   ( (reg32 *) DbgUart_mosi_s__0__HSIOM)
    
    #define DbgUart_MOSI_S_HSIOM_MASK      (DbgUart_mosi_s__0__HSIOM_MASK)
    #define DbgUart_MOSI_S_HSIOM_POS       (DbgUart_mosi_s__0__HSIOM_SHIFT)
    #define DbgUart_MOSI_S_HSIOM_SEL_GPIO  (DbgUart_mosi_s__0__HSIOM_GPIO)
    #define DbgUart_MOSI_S_HSIOM_SEL_SPI   (DbgUart_mosi_s__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_SLAVE_MOSI_PIN) */

#if (DbgUart_SPI_SLAVE_MISO_PIN)
    #define DbgUart_MISO_S_HSIOM_REG   (*(reg32 *) DbgUart_miso_s__0__HSIOM)
    #define DbgUart_MISO_S_HSIOM_PTR   ( (reg32 *) DbgUart_miso_s__0__HSIOM)
    
    #define DbgUart_MISO_S_HSIOM_MASK      (DbgUart_miso_s__0__HSIOM_MASK)
    #define DbgUart_MISO_S_HSIOM_POS       (DbgUart_miso_s__0__HSIOM_SHIFT)
    #define DbgUart_MISO_S_HSIOM_SEL_GPIO  (DbgUart_miso_s__0__HSIOM_GPIO)
    #define DbgUart_MISO_S_HSIOM_SEL_SPI   (DbgUart_miso_s__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_SLAVE_MISO_PIN) */

#if (DbgUart_SPI_MASTER_MISO_PIN)
    #define DbgUart_MISO_M_HSIOM_REG   (*(reg32 *) DbgUart_miso_m__0__HSIOM)
    #define DbgUart_MISO_M_HSIOM_PTR   ( (reg32 *) DbgUart_miso_m__0__HSIOM)
    
    #define DbgUart_MISO_M_HSIOM_MASK      (DbgUart_miso_m__0__HSIOM_MASK)
    #define DbgUart_MISO_M_HSIOM_POS       (DbgUart_miso_m__0__HSIOM_SHIFT)
    #define DbgUart_MISO_M_HSIOM_SEL_GPIO  (DbgUart_miso_m__0__HSIOM_GPIO)
    #define DbgUart_MISO_M_HSIOM_SEL_SPI   (DbgUart_miso_m__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_MASTER_MISO_PIN) */

#if (DbgUart_SPI_MASTER_MOSI_PIN)
    #define DbgUart_MOSI_M_HSIOM_REG   (*(reg32 *) DbgUart_mosi_m__0__HSIOM)
    #define DbgUart_MOSI_M_HSIOM_PTR   ( (reg32 *) DbgUart_mosi_m__0__HSIOM)
    
    #define DbgUart_MOSI_M_HSIOM_MASK      (DbgUart_mosi_m__0__HSIOM_MASK)
    #define DbgUart_MOSI_M_HSIOM_POS       (DbgUart_mosi_m__0__HSIOM_SHIFT)
    #define DbgUart_MOSI_M_HSIOM_SEL_GPIO  (DbgUart_mosi_m__0__HSIOM_GPIO)
    #define DbgUart_MOSI_M_HSIOM_SEL_SPI   (DbgUart_mosi_m__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_MASTER_MOSI_PIN) */

#if (DbgUart_SPI_MASTER_SCLK_PIN)
    #define DbgUart_SCLK_M_HSIOM_REG   (*(reg32 *) DbgUart_sclk_m__0__HSIOM)
    #define DbgUart_SCLK_M_HSIOM_PTR   ( (reg32 *) DbgUart_sclk_m__0__HSIOM)
    
    #define DbgUart_SCLK_M_HSIOM_MASK      (DbgUart_sclk_m__0__HSIOM_MASK)
    #define DbgUart_SCLK_M_HSIOM_POS       (DbgUart_sclk_m__0__HSIOM_SHIFT)
    #define DbgUart_SCLK_M_HSIOM_SEL_GPIO  (DbgUart_sclk_m__0__HSIOM_GPIO)
    #define DbgUart_SCLK_M_HSIOM_SEL_SPI   (DbgUart_sclk_m__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_MASTER_SCLK_PIN) */

#if (DbgUart_SPI_MASTER_SS0_PIN)
    #define DbgUart_SS0_M_HSIOM_REG    (*(reg32 *) DbgUart_ss0_m__0__HSIOM)
    #define DbgUart_SS0_M_HSIOM_PTR    ( (reg32 *) DbgUart_ss0_m__0__HSIOM)
    
    #define DbgUart_SS0_M_HSIOM_MASK       (DbgUart_ss0_m__0__HSIOM_MASK)
    #define DbgUart_SS0_M_HSIOM_POS        (DbgUart_ss0_m__0__HSIOM_SHIFT)
    #define DbgUart_SS0_M_HSIOM_SEL_GPIO   (DbgUart_ss0_m__0__HSIOM_GPIO)
    #define DbgUart_SS0_M_HSIOM_SEL_SPI    (DbgUart_ss0_m__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_MASTER_SS0_PIN) */

#if (DbgUart_SPI_MASTER_SS1_PIN)
    #define DbgUart_SS1_M_HSIOM_REG    (*(reg32 *) DbgUart_ss1_m__0__HSIOM)
    #define DbgUart_SS1_M_HSIOM_PTR    ( (reg32 *) DbgUart_ss1_m__0__HSIOM)
    
    #define DbgUart_SS1_M_HSIOM_MASK       (DbgUart_ss1_m__0__HSIOM_MASK)
    #define DbgUart_SS1_M_HSIOM_POS        (DbgUart_ss1_m__0__HSIOM_SHIFT)
    #define DbgUart_SS1_M_HSIOM_SEL_GPIO   (DbgUart_ss1_m__0__HSIOM_GPIO)
    #define DbgUart_SS1_M_HSIOM_SEL_SPI    (DbgUart_ss1_m__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_MASTER_SS1_PIN) */

#if (DbgUart_SPI_MASTER_SS2_PIN)
    #define DbgUart_SS2_M_HSIOM_REG    (*(reg32 *) DbgUart_ss2_m__0__HSIOM)
    #define DbgUart_SS2_M_HSIOM_PTR    ( (reg32 *) DbgUart_ss2_m__0__HSIOM)
    
    #define DbgUart_SS2_M_HSIOM_MASK       (DbgUart_ss2_m__0__HSIOM_MASK)
    #define DbgUart_SS2_M_HSIOM_POS        (DbgUart_ss2_m__0__HSIOM_SHIFT)
    #define DbgUart_SS2_M_HSIOM_SEL_GPIO   (DbgUart_ss2_m__0__HSIOM_GPIO)
    #define DbgUart_SS2_M_HSIOM_SEL_SPI    (DbgUart_ss2_m__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_MASTER_SS2_PIN) */

#if (DbgUart_SPI_MASTER_SS3_PIN)
    #define DbgUart_SS3_M_HSIOM_REG    (*(reg32 *) DbgUart_ss3_m__0__HSIOM)
    #define DbgUart_SS3_M_HSIOM_PTR    ( (reg32 *) DbgUart_ss3_m__0__HSIOM)
    
    #define DbgUart_SS3_M_HSIOM_MASK      (DbgUart_ss3_m__0__HSIOM_MASK)
    #define DbgUart_SS3_M_HSIOM_POS       (DbgUart_ss3_m__0__HSIOM_SHIFT)
    #define DbgUart_SS3_M_HSIOM_SEL_GPIO  (DbgUart_ss3_m__0__HSIOM_GPIO)
    #define DbgUart_SS3_M_HSIOM_SEL_SPI   (DbgUart_ss3_m__0__HSIOM_SPI)
#endif /* (DbgUart_SPI_MASTER_SS3_PIN) */

#if (DbgUart_UART_RX_PIN)
    #define DbgUart_RX_HSIOM_REG   (*(reg32 *) DbgUart_rx__0__HSIOM)
    #define DbgUart_RX_HSIOM_PTR   ( (reg32 *) DbgUart_rx__0__HSIOM)
    
    #define DbgUart_RX_HSIOM_MASK      (DbgUart_rx__0__HSIOM_MASK)
    #define DbgUart_RX_HSIOM_POS       (DbgUart_rx__0__HSIOM_SHIFT)
    #define DbgUart_RX_HSIOM_SEL_GPIO  (DbgUart_rx__0__HSIOM_GPIO)
    #define DbgUart_RX_HSIOM_SEL_UART  (DbgUart_rx__0__HSIOM_UART)
#endif /* (DbgUart_UART_RX_PIN) */

#if (DbgUart_UART_RX_WAKE_PIN)
    #define DbgUart_RX_WAKE_HSIOM_REG   (*(reg32 *) DbgUart_rx_wake__0__HSIOM)
    #define DbgUart_RX_WAKE_HSIOM_PTR   ( (reg32 *) DbgUart_rx_wake__0__HSIOM)
    
    #define DbgUart_RX_WAKE_HSIOM_MASK      (DbgUart_rx_wake__0__HSIOM_MASK)
    #define DbgUart_RX_WAKE_HSIOM_POS       (DbgUart_rx_wake__0__HSIOM_SHIFT)
    #define DbgUart_RX_WAKE_HSIOM_SEL_GPIO  (DbgUart_rx_wake__0__HSIOM_GPIO)
    #define DbgUart_RX_WAKE_HSIOM_SEL_UART  (DbgUart_rx_wake__0__HSIOM_UART)
#endif /* (DbgUart_UART_WAKE_RX_PIN) */

#if (DbgUart_UART_CTS_PIN)
    #define DbgUart_CTS_HSIOM_REG   (*(reg32 *) DbgUart_cts__0__HSIOM)
    #define DbgUart_CTS_HSIOM_PTR   ( (reg32 *) DbgUart_cts__0__HSIOM)
    
    #define DbgUart_CTS_HSIOM_MASK      (DbgUart_cts__0__HSIOM_MASK)
    #define DbgUart_CTS_HSIOM_POS       (DbgUart_cts__0__HSIOM_SHIFT)
    #define DbgUart_CTS_HSIOM_SEL_GPIO  (DbgUart_cts__0__HSIOM_GPIO)
    #define DbgUart_CTS_HSIOM_SEL_UART  (DbgUart_cts__0__HSIOM_UART)
#endif /* (DbgUart_UART_CTS_PIN) */

#if (DbgUart_UART_TX_PIN)
    #define DbgUart_TX_HSIOM_REG   (*(reg32 *) DbgUart_tx__0__HSIOM)
    #define DbgUart_TX_HSIOM_PTR   ( (reg32 *) DbgUart_tx__0__HSIOM)
    
    #define DbgUart_TX_HSIOM_MASK      (DbgUart_tx__0__HSIOM_MASK)
    #define DbgUart_TX_HSIOM_POS       (DbgUart_tx__0__HSIOM_SHIFT)
    #define DbgUart_TX_HSIOM_SEL_GPIO  (DbgUart_tx__0__HSIOM_GPIO)
    #define DbgUart_TX_HSIOM_SEL_UART  (DbgUart_tx__0__HSIOM_UART)
#endif /* (DbgUart_UART_TX_PIN) */

#if (DbgUart_UART_RX_TX_PIN)
    #define DbgUart_RX_TX_HSIOM_REG   (*(reg32 *) DbgUart_rx_tx__0__HSIOM)
    #define DbgUart_RX_TX_HSIOM_PTR   ( (reg32 *) DbgUart_rx_tx__0__HSIOM)
    
    #define DbgUart_RX_TX_HSIOM_MASK      (DbgUart_rx_tx__0__HSIOM_MASK)
    #define DbgUart_RX_TX_HSIOM_POS       (DbgUart_rx_tx__0__HSIOM_SHIFT)
    #define DbgUart_RX_TX_HSIOM_SEL_GPIO  (DbgUart_rx_tx__0__HSIOM_GPIO)
    #define DbgUart_RX_TX_HSIOM_SEL_UART  (DbgUart_rx_tx__0__HSIOM_UART)
#endif /* (DbgUart_UART_RX_TX_PIN) */

#if (DbgUart_UART_RTS_PIN)
    #define DbgUart_RTS_HSIOM_REG      (*(reg32 *) DbgUart_rts__0__HSIOM)
    #define DbgUart_RTS_HSIOM_PTR      ( (reg32 *) DbgUart_rts__0__HSIOM)
    
    #define DbgUart_RTS_HSIOM_MASK     (DbgUart_rts__0__HSIOM_MASK)
    #define DbgUart_RTS_HSIOM_POS      (DbgUart_rts__0__HSIOM_SHIFT)    
    #define DbgUart_RTS_HSIOM_SEL_GPIO (DbgUart_rts__0__HSIOM_GPIO)
    #define DbgUart_RTS_HSIOM_SEL_UART (DbgUart_rts__0__HSIOM_UART)    
#endif /* (DbgUart_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define DbgUart_HSIOM_DEF_SEL      (0x00u)
#define DbgUart_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for DbgUart_CY_SCBIP_V0 
* and DbgUart_CY_SCBIP_V1. It is not recommended to use them for 
* DbgUart_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define DbgUart_HSIOM_UART_SEL     (0x09u)
#define DbgUart_HSIOM_I2C_SEL      (0x0Eu)
#define DbgUart_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX   (0u)
#define DbgUart_RX_SCL_MOSI_PIN_INDEX       (0u)
#define DbgUart_TX_SDA_MISO_PIN_INDEX       (1u)
#define DbgUart_CTS_SCLK_PIN_INDEX       (2u)
#define DbgUart_RTS_SS0_PIN_INDEX       (3u)
#define DbgUart_SS1_PIN_INDEX                  (4u)
#define DbgUart_SS2_PIN_INDEX                  (5u)
#define DbgUart_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define DbgUart_RX_WAKE_SCL_MOSI_PIN_MASK ((uint32) 0x01u << DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX)
#define DbgUart_RX_SCL_MOSI_PIN_MASK     ((uint32) 0x01u << DbgUart_RX_SCL_MOSI_PIN_INDEX)
#define DbgUart_TX_SDA_MISO_PIN_MASK     ((uint32) 0x01u << DbgUart_TX_SDA_MISO_PIN_INDEX)
#define DbgUart_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << DbgUart_CTS_SCLK_PIN_INDEX)
#define DbgUart_RTS_SS0_PIN_MASK     ((uint32) 0x01u << DbgUart_RTS_SS0_PIN_INDEX)
#define DbgUart_SS1_PIN_MASK                ((uint32) 0x01u << DbgUart_SS1_PIN_INDEX)
#define DbgUart_SS2_PIN_MASK                ((uint32) 0x01u << DbgUart_SS2_PIN_INDEX)
#define DbgUart_SS3_PIN_MASK                ((uint32) 0x01u << DbgUart_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define DbgUart_INTCFG_TYPE_MASK           (0x03u)
#define DbgUart_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define DbgUart_PIN_DM_ALG_HIZ  (0u)
#define DbgUart_PIN_DM_DIG_HIZ  (1u)
#define DbgUart_PIN_DM_OD_LO    (4u)
#define DbgUart_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define DbgUart_DM_MASK    (0x7u)
#define DbgUart_DM_SIZE    (3u)
#define DbgUart_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) DbgUart_DM_MASK << (DbgUart_DM_SIZE * (pos)))) >> \
                                                              (DbgUart_DM_SIZE * (pos)) )

#if (DbgUart_TX_SDA_MISO_PIN)
    #define DbgUart_CHECK_TX_SDA_MISO_PIN_USED \
                (DbgUart_PIN_DM_ALG_HIZ != \
                    DbgUart_GET_P4_PIN_DM(DbgUart_uart_tx_i2c_sda_spi_miso_PC, \
                                                   DbgUart_uart_tx_i2c_sda_spi_miso_SHIFT))
#endif /* (DbgUart_TX_SDA_MISO_PIN) */

#if (DbgUart_RTS_SS0_PIN)
    #define DbgUart_CHECK_RTS_SS0_PIN_USED \
                (DbgUart_PIN_DM_ALG_HIZ != \
                    DbgUart_GET_P4_PIN_DM(DbgUart_uart_rts_spi_ss0_PC, \
                                                   DbgUart_uart_rts_spi_ss0_SHIFT))
#endif /* (DbgUart_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define DbgUart_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define DbgUart_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define DbgUart_SET_HSIOM_SEL(reg, mask, pos, sel) DbgUart_SET_REGISTER_BITS(reg, mask, pos, sel)
#define DbgUart_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        DbgUart_SET_REGISTER_BITS(reg, mask, pos, intType)
#define DbgUart_SET_INP_DIS(reg, mask, val) DbgUart_SET_REGISTER_BIT(reg, mask, val)

/* DbgUart_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  DbgUart_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (DbgUart_CY_SCBIP_V0)
#if (DbgUart_I2C_PINS)
    #define DbgUart_SET_I2C_SCL_DR(val) DbgUart_scl_Write(val)

    #define DbgUart_SET_I2C_SCL_HSIOM_SEL(sel) \
                          DbgUart_SET_HSIOM_SEL(DbgUart_SCL_HSIOM_REG,  \
                                                         DbgUart_SCL_HSIOM_MASK, \
                                                         DbgUart_SCL_HSIOM_POS,  \
                                                         (sel))
    #define DbgUart_WAIT_SCL_SET_HIGH  (0u == DbgUart_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
    #define DbgUart_SET_I2C_SCL_DR(val) \
                            DbgUart_uart_rx_wake_i2c_scl_spi_mosi_Write(val)

    #define DbgUart_SET_I2C_SCL_HSIOM_SEL(sel) \
                    DbgUart_SET_HSIOM_SEL(DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG,  \
                                                   DbgUart_RX_WAKE_SCL_MOSI_HSIOM_MASK, \
                                                   DbgUart_RX_WAKE_SCL_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define DbgUart_WAIT_SCL_SET_HIGH  (0u == DbgUart_uart_rx_wake_i2c_scl_spi_mosi_Read())

#elif (DbgUart_RX_SCL_MOSI_PIN)
    #define DbgUart_SET_I2C_SCL_DR(val) \
                            DbgUart_uart_rx_i2c_scl_spi_mosi_Write(val)


    #define DbgUart_SET_I2C_SCL_HSIOM_SEL(sel) \
                            DbgUart_SET_HSIOM_SEL(DbgUart_RX_SCL_MOSI_HSIOM_REG,  \
                                                           DbgUart_RX_SCL_MOSI_HSIOM_MASK, \
                                                           DbgUart_RX_SCL_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define DbgUart_WAIT_SCL_SET_HIGH  (0u == DbgUart_uart_rx_i2c_scl_spi_mosi_Read())

#else
    #define DbgUart_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define DbgUart_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define DbgUart_WAIT_SCL_SET_HIGH  (0u)
#endif /* (DbgUart_I2C_PINS) */

/* SCB I2C: sda signal */
#if (DbgUart_I2C_PINS)
    #define DbgUart_WAIT_SDA_SET_HIGH  (0u == DbgUart_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (DbgUart_TX_SDA_MISO_PIN)
    #define DbgUart_WAIT_SDA_SET_HIGH  (0u == DbgUart_uart_tx_i2c_sda_spi_miso_Read())
#else
    #define DbgUart_WAIT_SDA_SET_HIGH  (0u)
#endif /* (DbgUart_MOSI_SCL_RX_PIN) */
#endif /* (DbgUart_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (DbgUart_RX_SCL_MOSI_PIN)
    #define DbgUart_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (DbgUart_RX_WAKE_SCL_MOSI_PIN)
    #define DbgUart_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) DbgUart_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(DbgUart_UART_RX_WAKE_PIN)
    #define DbgUart_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) DbgUart_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (DbgUart_RX_SCL_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define DbgUart_REMOVE_MOSI_SCL_RX_WAKE_PIN    DbgUart_REMOVE_RX_WAKE_SCL_MOSI_PIN
#define DbgUart_REMOVE_MOSI_SCL_RX_PIN         DbgUart_REMOVE_RX_SCL_MOSI_PIN
#define DbgUart_REMOVE_MISO_SDA_TX_PIN         DbgUart_REMOVE_TX_SDA_MISO_PIN
#ifndef DbgUart_REMOVE_SCLK_PIN
#define DbgUart_REMOVE_SCLK_PIN                DbgUart_REMOVE_CTS_SCLK_PIN
#endif /* DbgUart_REMOVE_SCLK_PIN */
#ifndef DbgUart_REMOVE_SS0_PIN
#define DbgUart_REMOVE_SS0_PIN                 DbgUart_REMOVE_RTS_SS0_PIN
#endif /* DbgUart_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define DbgUart_MOSI_SCL_RX_WAKE_PIN   DbgUart_RX_WAKE_SCL_MOSI_PIN
#define DbgUart_MOSI_SCL_RX_PIN        DbgUart_RX_SCL_MOSI_PIN
#define DbgUart_MISO_SDA_TX_PIN        DbgUart_TX_SDA_MISO_PIN
#ifndef DbgUart_SCLK_PIN
#define DbgUart_SCLK_PIN               DbgUart_CTS_SCLK_PIN
#endif /* DbgUart_SCLK_PIN */
#ifndef DbgUart_SS0_PIN
#define DbgUart_SS0_PIN                DbgUart_RTS_SS0_PIN
#endif /* DbgUart_SS0_PIN */

#if (DbgUart_MOSI_SCL_RX_WAKE_PIN)
    #define DbgUart_MOSI_SCL_RX_WAKE_HSIOM_REG     DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define DbgUart_MOSI_SCL_RX_WAKE_HSIOM_PTR     DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define DbgUart_MOSI_SCL_RX_WAKE_HSIOM_MASK    DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define DbgUart_MOSI_SCL_RX_WAKE_HSIOM_POS     DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define DbgUart_MOSI_SCL_RX_WAKE_INTCFG_REG    DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define DbgUart_MOSI_SCL_RX_WAKE_INTCFG_PTR    DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define DbgUart_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define DbgUart_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  DbgUart_RX_WAKE_SCL_MOSI_HSIOM_REG
#endif /* (DbgUart_RX_WAKE_SCL_MOSI_PIN) */

#if (DbgUart_MOSI_SCL_RX_PIN)
    #define DbgUart_MOSI_SCL_RX_HSIOM_REG      DbgUart_RX_SCL_MOSI_HSIOM_REG
    #define DbgUart_MOSI_SCL_RX_HSIOM_PTR      DbgUart_RX_SCL_MOSI_HSIOM_PTR
    #define DbgUart_MOSI_SCL_RX_HSIOM_MASK     DbgUart_RX_SCL_MOSI_HSIOM_MASK
    #define DbgUart_MOSI_SCL_RX_HSIOM_POS      DbgUart_RX_SCL_MOSI_HSIOM_POS
#endif /* (DbgUart_MOSI_SCL_RX_PIN) */

#if (DbgUart_MISO_SDA_TX_PIN)
    #define DbgUart_MISO_SDA_TX_HSIOM_REG      DbgUart_TX_SDA_MISO_HSIOM_REG
    #define DbgUart_MISO_SDA_TX_HSIOM_PTR      DbgUart_TX_SDA_MISO_HSIOM_REG
    #define DbgUart_MISO_SDA_TX_HSIOM_MASK     DbgUart_TX_SDA_MISO_HSIOM_REG
    #define DbgUart_MISO_SDA_TX_HSIOM_POS      DbgUart_TX_SDA_MISO_HSIOM_REG
#endif /* (DbgUart_MISO_SDA_TX_PIN_PIN) */

#if (DbgUart_SCLK_PIN)
    #ifndef DbgUart_SCLK_HSIOM_REG
    #define DbgUart_SCLK_HSIOM_REG     DbgUart_CTS_SCLK_HSIOM_REG
    #define DbgUart_SCLK_HSIOM_PTR     DbgUart_CTS_SCLK_HSIOM_PTR
    #define DbgUart_SCLK_HSIOM_MASK    DbgUart_CTS_SCLK_HSIOM_MASK
    #define DbgUart_SCLK_HSIOM_POS     DbgUart_CTS_SCLK_HSIOM_POS
    #endif /* DbgUart_SCLK_HSIOM_REG */
#endif /* (DbgUart_SCLK_PIN) */

#if (DbgUart_SS0_PIN)
    #ifndef DbgUart_SS0_HSIOM_REG
    #define DbgUart_SS0_HSIOM_REG      DbgUart_RTS_SS0_HSIOM_REG
    #define DbgUart_SS0_HSIOM_PTR      DbgUart_RTS_SS0_HSIOM_PTR
    #define DbgUart_SS0_HSIOM_MASK     DbgUart_RTS_SS0_HSIOM_MASK
    #define DbgUart_SS0_HSIOM_POS      DbgUart_RTS_SS0_HSIOM_POS
    #endif /* DbgUart_SS0_HSIOM_REG */
#endif /* (DbgUart_SS0_PIN) */

#define DbgUart_MOSI_SCL_RX_WAKE_PIN_INDEX DbgUart_RX_WAKE_SCL_MOSI_PIN_INDEX
#define DbgUart_MOSI_SCL_RX_PIN_INDEX      DbgUart_RX_SCL_MOSI_PIN_INDEX
#define DbgUart_MISO_SDA_TX_PIN_INDEX      DbgUart_TX_SDA_MISO_PIN_INDEX
#ifndef DbgUart_SCLK_PIN_INDEX
#define DbgUart_SCLK_PIN_INDEX             DbgUart_CTS_SCLK_PIN_INDEX
#endif /* DbgUart_SCLK_PIN_INDEX */
#ifndef DbgUart_SS0_PIN_INDEX
#define DbgUart_SS0_PIN_INDEX              DbgUart_RTS_SS0_PIN_INDEX
#endif /* DbgUart_SS0_PIN_INDEX */

#define DbgUart_MOSI_SCL_RX_WAKE_PIN_MASK DbgUart_RX_WAKE_SCL_MOSI_PIN_MASK
#define DbgUart_MOSI_SCL_RX_PIN_MASK      DbgUart_RX_SCL_MOSI_PIN_MASK
#define DbgUart_MISO_SDA_TX_PIN_MASK      DbgUart_TX_SDA_MISO_PIN_MASK
#ifndef DbgUart_SCLK_PIN_MASK
#define DbgUart_SCLK_PIN_MASK             DbgUart_CTS_SCLK_PIN_MASK
#endif /* DbgUart_SCLK_PIN_MASK */
#ifndef DbgUart_SS0_PIN_MASK
#define DbgUart_SS0_PIN_MASK              DbgUart_RTS_SS0_PIN_MASK
#endif /* DbgUart_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_DbgUart_H) */


/* [] END OF FILE */
