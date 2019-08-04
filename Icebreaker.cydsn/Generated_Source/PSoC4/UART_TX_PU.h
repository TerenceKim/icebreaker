/*******************************************************************************
* File Name: UART_TX_PU.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_UART_TX_PU_H) /* Pins UART_TX_PU_H */
#define CY_PINS_UART_TX_PU_H

#include "cytypes.h"
#include "cyfitter.h"
#include "UART_TX_PU_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} UART_TX_PU_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   UART_TX_PU_Read(void);
void    UART_TX_PU_Write(uint8 value);
uint8   UART_TX_PU_ReadDataReg(void);
#if defined(UART_TX_PU__PC) || (CY_PSOC4_4200L) 
    void    UART_TX_PU_SetDriveMode(uint8 mode);
#endif
void    UART_TX_PU_SetInterruptMode(uint16 position, uint16 mode);
uint8   UART_TX_PU_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void UART_TX_PU_Sleep(void); 
void UART_TX_PU_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(UART_TX_PU__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define UART_TX_PU_DRIVE_MODE_BITS        (3)
    #define UART_TX_PU_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - UART_TX_PU_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the UART_TX_PU_SetDriveMode() function.
         *  @{
         */
        #define UART_TX_PU_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define UART_TX_PU_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define UART_TX_PU_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define UART_TX_PU_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define UART_TX_PU_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define UART_TX_PU_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define UART_TX_PU_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define UART_TX_PU_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define UART_TX_PU_MASK               UART_TX_PU__MASK
#define UART_TX_PU_SHIFT              UART_TX_PU__SHIFT
#define UART_TX_PU_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in UART_TX_PU_SetInterruptMode() function.
     *  @{
     */
        #define UART_TX_PU_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define UART_TX_PU_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define UART_TX_PU_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define UART_TX_PU_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(UART_TX_PU__SIO)
    #define UART_TX_PU_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(UART_TX_PU__PC) && (CY_PSOC4_4200L)
    #define UART_TX_PU_USBIO_ENABLE               ((uint32)0x80000000u)
    #define UART_TX_PU_USBIO_DISABLE              ((uint32)(~UART_TX_PU_USBIO_ENABLE))
    #define UART_TX_PU_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define UART_TX_PU_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define UART_TX_PU_USBIO_ENTER_SLEEP          ((uint32)((1u << UART_TX_PU_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << UART_TX_PU_USBIO_SUSPEND_DEL_SHIFT)))
    #define UART_TX_PU_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << UART_TX_PU_USBIO_SUSPEND_SHIFT)))
    #define UART_TX_PU_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << UART_TX_PU_USBIO_SUSPEND_DEL_SHIFT)))
    #define UART_TX_PU_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(UART_TX_PU__PC)
    /* Port Configuration */
    #define UART_TX_PU_PC                 (* (reg32 *) UART_TX_PU__PC)
#endif
/* Pin State */
#define UART_TX_PU_PS                     (* (reg32 *) UART_TX_PU__PS)
/* Data Register */
#define UART_TX_PU_DR                     (* (reg32 *) UART_TX_PU__DR)
/* Input Buffer Disable Override */
#define UART_TX_PU_INP_DIS                (* (reg32 *) UART_TX_PU__PC2)

/* Interrupt configuration Registers */
#define UART_TX_PU_INTCFG                 (* (reg32 *) UART_TX_PU__INTCFG)
#define UART_TX_PU_INTSTAT                (* (reg32 *) UART_TX_PU__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define UART_TX_PU_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(UART_TX_PU__SIO)
    #define UART_TX_PU_SIO_REG            (* (reg32 *) UART_TX_PU__SIO)
#endif /* (UART_TX_PU__SIO_CFG) */

/* USBIO registers */
#if !defined(UART_TX_PU__PC) && (CY_PSOC4_4200L)
    #define UART_TX_PU_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define UART_TX_PU_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define UART_TX_PU_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define UART_TX_PU_DRIVE_MODE_SHIFT       (0x00u)
#define UART_TX_PU_DRIVE_MODE_MASK        (0x07u << UART_TX_PU_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins UART_TX_PU_H */


/* [] END OF FILE */
