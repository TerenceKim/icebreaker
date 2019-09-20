/*******************************************************************************
* File Name: RF_MCLK.h  
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

#if !defined(CY_PINS_RF_MCLK_H) /* Pins RF_MCLK_H */
#define CY_PINS_RF_MCLK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RF_MCLK_aliases.h"


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
} RF_MCLK_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RF_MCLK_Read(void);
void    RF_MCLK_Write(uint8 value);
uint8   RF_MCLK_ReadDataReg(void);
#if defined(RF_MCLK__PC) || (CY_PSOC4_4200L) 
    void    RF_MCLK_SetDriveMode(uint8 mode);
#endif
void    RF_MCLK_SetInterruptMode(uint16 position, uint16 mode);
uint8   RF_MCLK_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RF_MCLK_Sleep(void); 
void RF_MCLK_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RF_MCLK__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RF_MCLK_DRIVE_MODE_BITS        (3)
    #define RF_MCLK_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RF_MCLK_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RF_MCLK_SetDriveMode() function.
         *  @{
         */
        #define RF_MCLK_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RF_MCLK_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RF_MCLK_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RF_MCLK_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RF_MCLK_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RF_MCLK_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RF_MCLK_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RF_MCLK_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RF_MCLK_MASK               RF_MCLK__MASK
#define RF_MCLK_SHIFT              RF_MCLK__SHIFT
#define RF_MCLK_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RF_MCLK_SetInterruptMode() function.
     *  @{
     */
        #define RF_MCLK_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RF_MCLK_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RF_MCLK_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RF_MCLK_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RF_MCLK__SIO)
    #define RF_MCLK_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RF_MCLK__PC) && (CY_PSOC4_4200L)
    #define RF_MCLK_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RF_MCLK_USBIO_DISABLE              ((uint32)(~RF_MCLK_USBIO_ENABLE))
    #define RF_MCLK_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RF_MCLK_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RF_MCLK_USBIO_ENTER_SLEEP          ((uint32)((1u << RF_MCLK_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RF_MCLK_USBIO_SUSPEND_DEL_SHIFT)))
    #define RF_MCLK_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RF_MCLK_USBIO_SUSPEND_SHIFT)))
    #define RF_MCLK_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RF_MCLK_USBIO_SUSPEND_DEL_SHIFT)))
    #define RF_MCLK_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RF_MCLK__PC)
    /* Port Configuration */
    #define RF_MCLK_PC                 (* (reg32 *) RF_MCLK__PC)
#endif
/* Pin State */
#define RF_MCLK_PS                     (* (reg32 *) RF_MCLK__PS)
/* Data Register */
#define RF_MCLK_DR                     (* (reg32 *) RF_MCLK__DR)
/* Input Buffer Disable Override */
#define RF_MCLK_INP_DIS                (* (reg32 *) RF_MCLK__PC2)

/* Interrupt configuration Registers */
#define RF_MCLK_INTCFG                 (* (reg32 *) RF_MCLK__INTCFG)
#define RF_MCLK_INTSTAT                (* (reg32 *) RF_MCLK__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RF_MCLK_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RF_MCLK__SIO)
    #define RF_MCLK_SIO_REG            (* (reg32 *) RF_MCLK__SIO)
#endif /* (RF_MCLK__SIO_CFG) */

/* USBIO registers */
#if !defined(RF_MCLK__PC) && (CY_PSOC4_4200L)
    #define RF_MCLK_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RF_MCLK_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RF_MCLK_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RF_MCLK_DRIVE_MODE_SHIFT       (0x00u)
#define RF_MCLK_DRIVE_MODE_MASK        (0x07u << RF_MCLK_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RF_MCLK_H */


/* [] END OF FILE */
