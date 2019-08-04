/*******************************************************************************
* File Name: PD.h  
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

#if !defined(CY_PINS_PD_H) /* Pins PD_H */
#define CY_PINS_PD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "PD_aliases.h"


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
} PD_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   PD_Read(void);
void    PD_Write(uint8 value);
uint8   PD_ReadDataReg(void);
#if defined(PD__PC) || (CY_PSOC4_4200L) 
    void    PD_SetDriveMode(uint8 mode);
#endif
void    PD_SetInterruptMode(uint16 position, uint16 mode);
uint8   PD_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void PD_Sleep(void); 
void PD_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(PD__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define PD_DRIVE_MODE_BITS        (3)
    #define PD_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - PD_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the PD_SetDriveMode() function.
         *  @{
         */
        #define PD_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define PD_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define PD_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define PD_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define PD_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define PD_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define PD_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define PD_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define PD_MASK               PD__MASK
#define PD_SHIFT              PD__SHIFT
#define PD_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in PD_SetInterruptMode() function.
     *  @{
     */
        #define PD_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define PD_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define PD_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define PD_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(PD__SIO)
    #define PD_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(PD__PC) && (CY_PSOC4_4200L)
    #define PD_USBIO_ENABLE               ((uint32)0x80000000u)
    #define PD_USBIO_DISABLE              ((uint32)(~PD_USBIO_ENABLE))
    #define PD_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define PD_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define PD_USBIO_ENTER_SLEEP          ((uint32)((1u << PD_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << PD_USBIO_SUSPEND_DEL_SHIFT)))
    #define PD_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << PD_USBIO_SUSPEND_SHIFT)))
    #define PD_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << PD_USBIO_SUSPEND_DEL_SHIFT)))
    #define PD_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(PD__PC)
    /* Port Configuration */
    #define PD_PC                 (* (reg32 *) PD__PC)
#endif
/* Pin State */
#define PD_PS                     (* (reg32 *) PD__PS)
/* Data Register */
#define PD_DR                     (* (reg32 *) PD__DR)
/* Input Buffer Disable Override */
#define PD_INP_DIS                (* (reg32 *) PD__PC2)

/* Interrupt configuration Registers */
#define PD_INTCFG                 (* (reg32 *) PD__INTCFG)
#define PD_INTSTAT                (* (reg32 *) PD__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define PD_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(PD__SIO)
    #define PD_SIO_REG            (* (reg32 *) PD__SIO)
#endif /* (PD__SIO_CFG) */

/* USBIO registers */
#if !defined(PD__PC) && (CY_PSOC4_4200L)
    #define PD_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define PD_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define PD_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define PD_DRIVE_MODE_SHIFT       (0x00u)
#define PD_DRIVE_MODE_MASK        (0x07u << PD_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins PD_H */


/* [] END OF FILE */
