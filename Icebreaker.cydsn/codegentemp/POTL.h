/*******************************************************************************
* File Name: POTL.h  
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

#if !defined(CY_PINS_POTL_H) /* Pins POTL_H */
#define CY_PINS_POTL_H

#include "cytypes.h"
#include "cyfitter.h"
#include "POTL_aliases.h"


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
} POTL_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   POTL_Read(void);
void    POTL_Write(uint8 value);
uint8   POTL_ReadDataReg(void);
#if defined(POTL__PC) || (CY_PSOC4_4200L) 
    void    POTL_SetDriveMode(uint8 mode);
#endif
void    POTL_SetInterruptMode(uint16 position, uint16 mode);
uint8   POTL_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void POTL_Sleep(void); 
void POTL_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(POTL__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define POTL_DRIVE_MODE_BITS        (3)
    #define POTL_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - POTL_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the POTL_SetDriveMode() function.
         *  @{
         */
        #define POTL_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define POTL_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define POTL_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define POTL_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define POTL_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define POTL_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define POTL_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define POTL_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define POTL_MASK               POTL__MASK
#define POTL_SHIFT              POTL__SHIFT
#define POTL_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in POTL_SetInterruptMode() function.
     *  @{
     */
        #define POTL_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define POTL_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define POTL_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define POTL_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(POTL__SIO)
    #define POTL_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(POTL__PC) && (CY_PSOC4_4200L)
    #define POTL_USBIO_ENABLE               ((uint32)0x80000000u)
    #define POTL_USBIO_DISABLE              ((uint32)(~POTL_USBIO_ENABLE))
    #define POTL_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define POTL_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define POTL_USBIO_ENTER_SLEEP          ((uint32)((1u << POTL_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << POTL_USBIO_SUSPEND_DEL_SHIFT)))
    #define POTL_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << POTL_USBIO_SUSPEND_SHIFT)))
    #define POTL_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << POTL_USBIO_SUSPEND_DEL_SHIFT)))
    #define POTL_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(POTL__PC)
    /* Port Configuration */
    #define POTL_PC                 (* (reg32 *) POTL__PC)
#endif
/* Pin State */
#define POTL_PS                     (* (reg32 *) POTL__PS)
/* Data Register */
#define POTL_DR                     (* (reg32 *) POTL__DR)
/* Input Buffer Disable Override */
#define POTL_INP_DIS                (* (reg32 *) POTL__PC2)

/* Interrupt configuration Registers */
#define POTL_INTCFG                 (* (reg32 *) POTL__INTCFG)
#define POTL_INTSTAT                (* (reg32 *) POTL__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define POTL_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(POTL__SIO)
    #define POTL_SIO_REG            (* (reg32 *) POTL__SIO)
#endif /* (POTL__SIO_CFG) */

/* USBIO registers */
#if !defined(POTL__PC) && (CY_PSOC4_4200L)
    #define POTL_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define POTL_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define POTL_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define POTL_DRIVE_MODE_SHIFT       (0x00u)
#define POTL_DRIVE_MODE_MASK        (0x07u << POTL_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins POTL_H */


/* [] END OF FILE */
