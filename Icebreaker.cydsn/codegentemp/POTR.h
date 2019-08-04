/*******************************************************************************
* File Name: POTR.h  
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

#if !defined(CY_PINS_POTR_H) /* Pins POTR_H */
#define CY_PINS_POTR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "POTR_aliases.h"


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
} POTR_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   POTR_Read(void);
void    POTR_Write(uint8 value);
uint8   POTR_ReadDataReg(void);
#if defined(POTR__PC) || (CY_PSOC4_4200L) 
    void    POTR_SetDriveMode(uint8 mode);
#endif
void    POTR_SetInterruptMode(uint16 position, uint16 mode);
uint8   POTR_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void POTR_Sleep(void); 
void POTR_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(POTR__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define POTR_DRIVE_MODE_BITS        (3)
    #define POTR_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - POTR_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the POTR_SetDriveMode() function.
         *  @{
         */
        #define POTR_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define POTR_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define POTR_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define POTR_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define POTR_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define POTR_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define POTR_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define POTR_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define POTR_MASK               POTR__MASK
#define POTR_SHIFT              POTR__SHIFT
#define POTR_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in POTR_SetInterruptMode() function.
     *  @{
     */
        #define POTR_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define POTR_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define POTR_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define POTR_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(POTR__SIO)
    #define POTR_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(POTR__PC) && (CY_PSOC4_4200L)
    #define POTR_USBIO_ENABLE               ((uint32)0x80000000u)
    #define POTR_USBIO_DISABLE              ((uint32)(~POTR_USBIO_ENABLE))
    #define POTR_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define POTR_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define POTR_USBIO_ENTER_SLEEP          ((uint32)((1u << POTR_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << POTR_USBIO_SUSPEND_DEL_SHIFT)))
    #define POTR_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << POTR_USBIO_SUSPEND_SHIFT)))
    #define POTR_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << POTR_USBIO_SUSPEND_DEL_SHIFT)))
    #define POTR_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(POTR__PC)
    /* Port Configuration */
    #define POTR_PC                 (* (reg32 *) POTR__PC)
#endif
/* Pin State */
#define POTR_PS                     (* (reg32 *) POTR__PS)
/* Data Register */
#define POTR_DR                     (* (reg32 *) POTR__DR)
/* Input Buffer Disable Override */
#define POTR_INP_DIS                (* (reg32 *) POTR__PC2)

/* Interrupt configuration Registers */
#define POTR_INTCFG                 (* (reg32 *) POTR__INTCFG)
#define POTR_INTSTAT                (* (reg32 *) POTR__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define POTR_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(POTR__SIO)
    #define POTR_SIO_REG            (* (reg32 *) POTR__SIO)
#endif /* (POTR__SIO_CFG) */

/* USBIO registers */
#if !defined(POTR__PC) && (CY_PSOC4_4200L)
    #define POTR_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define POTR_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define POTR_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define POTR_DRIVE_MODE_SHIFT       (0x00u)
#define POTR_DRIVE_MODE_MASK        (0x07u << POTR_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins POTR_H */


/* [] END OF FILE */
