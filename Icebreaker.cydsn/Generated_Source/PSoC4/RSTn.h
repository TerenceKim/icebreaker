/*******************************************************************************
* File Name: RSTn.h  
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

#if !defined(CY_PINS_RSTn_H) /* Pins RSTn_H */
#define CY_PINS_RSTn_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RSTn_aliases.h"


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
} RSTn_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RSTn_Read(void);
void    RSTn_Write(uint8 value);
uint8   RSTn_ReadDataReg(void);
#if defined(RSTn__PC) || (CY_PSOC4_4200L) 
    void    RSTn_SetDriveMode(uint8 mode);
#endif
void    RSTn_SetInterruptMode(uint16 position, uint16 mode);
uint8   RSTn_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RSTn_Sleep(void); 
void RSTn_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RSTn__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RSTn_DRIVE_MODE_BITS        (3)
    #define RSTn_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RSTn_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RSTn_SetDriveMode() function.
         *  @{
         */
        #define RSTn_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RSTn_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RSTn_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RSTn_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RSTn_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RSTn_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RSTn_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RSTn_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RSTn_MASK               RSTn__MASK
#define RSTn_SHIFT              RSTn__SHIFT
#define RSTn_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RSTn_SetInterruptMode() function.
     *  @{
     */
        #define RSTn_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RSTn_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RSTn_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RSTn_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RSTn__SIO)
    #define RSTn_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RSTn__PC) && (CY_PSOC4_4200L)
    #define RSTn_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RSTn_USBIO_DISABLE              ((uint32)(~RSTn_USBIO_ENABLE))
    #define RSTn_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RSTn_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RSTn_USBIO_ENTER_SLEEP          ((uint32)((1u << RSTn_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RSTn_USBIO_SUSPEND_DEL_SHIFT)))
    #define RSTn_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RSTn_USBIO_SUSPEND_SHIFT)))
    #define RSTn_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RSTn_USBIO_SUSPEND_DEL_SHIFT)))
    #define RSTn_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RSTn__PC)
    /* Port Configuration */
    #define RSTn_PC                 (* (reg32 *) RSTn__PC)
#endif
/* Pin State */
#define RSTn_PS                     (* (reg32 *) RSTn__PS)
/* Data Register */
#define RSTn_DR                     (* (reg32 *) RSTn__DR)
/* Input Buffer Disable Override */
#define RSTn_INP_DIS                (* (reg32 *) RSTn__PC2)

/* Interrupt configuration Registers */
#define RSTn_INTCFG                 (* (reg32 *) RSTn__INTCFG)
#define RSTn_INTSTAT                (* (reg32 *) RSTn__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RSTn_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RSTn__SIO)
    #define RSTn_SIO_REG            (* (reg32 *) RSTn__SIO)
#endif /* (RSTn__SIO_CFG) */

/* USBIO registers */
#if !defined(RSTn__PC) && (CY_PSOC4_4200L)
    #define RSTn_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RSTn_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RSTn_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RSTn_DRIVE_MODE_SHIFT       (0x00u)
#define RSTn_DRIVE_MODE_MASK        (0x07u << RSTn_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RSTn_H */


/* [] END OF FILE */
