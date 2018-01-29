/*******************************************************************************
* File Name: Async_Feedback_Counter.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_Async_Feedback_Counter_H)
#define CY_Timer_v2_60_Async_Feedback_Counter_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Async_Feedback_Counter_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define Async_Feedback_Counter_Resolution                 8u
#define Async_Feedback_Counter_UsingFixedFunction         0u
#define Async_Feedback_Counter_UsingHWCaptureCounter      0u
#define Async_Feedback_Counter_SoftwareCaptureMode        0u
#define Async_Feedback_Counter_SoftwareTriggerMode        0u
#define Async_Feedback_Counter_UsingHWEnable              0u
#define Async_Feedback_Counter_EnableTriggerMode          0u
#define Async_Feedback_Counter_InterruptOnCaptureCount    0u
#define Async_Feedback_Counter_RunModeUsed                0u
#define Async_Feedback_Counter_ControlRegRemoved          0u

#if defined(Async_Feedback_Counter_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (Async_Feedback_Counter_UsingFixedFunction)
    #define Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End Async_Feedback_Counter_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!Async_Feedback_Counter_UsingFixedFunction)

        uint8 TimerUdb;
        uint8 InterruptMaskValue;
        #if (Async_Feedback_Counter_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}Async_Feedback_Counter_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    Async_Feedback_Counter_Start(void) ;
void    Async_Feedback_Counter_Stop(void) ;

void    Async_Feedback_Counter_SetInterruptMode(uint8 interruptMode) ;
uint8   Async_Feedback_Counter_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define Async_Feedback_Counter_GetInterruptSource() Async_Feedback_Counter_ReadStatusRegister()

#if(!Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED)
    uint8   Async_Feedback_Counter_ReadControlRegister(void) ;
    void    Async_Feedback_Counter_WriteControlRegister(uint8 control) ;
#endif /* (!Async_Feedback_Counter_UDB_CONTROL_REG_REMOVED) */

uint8  Async_Feedback_Counter_ReadPeriod(void) ;
void    Async_Feedback_Counter_WritePeriod(uint8 period) ;
uint8  Async_Feedback_Counter_ReadCounter(void) ;
void    Async_Feedback_Counter_WriteCounter(uint8 counter) ;
uint8  Async_Feedback_Counter_ReadCapture(void) ;
void    Async_Feedback_Counter_SoftwareCapture(void) ;

#if(!Async_Feedback_Counter_UsingFixedFunction) /* UDB Prototypes */
    #if (Async_Feedback_Counter_SoftwareCaptureMode)
        void    Async_Feedback_Counter_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!Async_Feedback_Counter_UsingFixedFunction) */

    #if (Async_Feedback_Counter_SoftwareTriggerMode)
        void    Async_Feedback_Counter_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (Async_Feedback_Counter_SoftwareTriggerMode) */

    #if (Async_Feedback_Counter_EnableTriggerMode)
        void    Async_Feedback_Counter_EnableTrigger(void) ;
        void    Async_Feedback_Counter_DisableTrigger(void) ;
    #endif /* (Async_Feedback_Counter_EnableTriggerMode) */


    #if(Async_Feedback_Counter_InterruptOnCaptureCount)
        void    Async_Feedback_Counter_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (Async_Feedback_Counter_InterruptOnCaptureCount) */

    #if (Async_Feedback_Counter_UsingHWCaptureCounter)
        void    Async_Feedback_Counter_SetCaptureCount(uint8 captureCount) ;
        uint8   Async_Feedback_Counter_ReadCaptureCount(void) ;
    #endif /* (Async_Feedback_Counter_UsingHWCaptureCounter) */

    void Async_Feedback_Counter_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void Async_Feedback_Counter_Init(void)          ;
void Async_Feedback_Counter_Enable(void)        ;
void Async_Feedback_Counter_SaveConfig(void)    ;
void Async_Feedback_Counter_RestoreConfig(void) ;
void Async_Feedback_Counter_Sleep(void)         ;
void Async_Feedback_Counter_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define Async_Feedback_Counter__B_TIMER__CM_NONE 0
#define Async_Feedback_Counter__B_TIMER__CM_RISINGEDGE 1
#define Async_Feedback_Counter__B_TIMER__CM_FALLINGEDGE 2
#define Async_Feedback_Counter__B_TIMER__CM_EITHEREDGE 3
#define Async_Feedback_Counter__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define Async_Feedback_Counter__B_TIMER__TM_NONE 0x00u
#define Async_Feedback_Counter__B_TIMER__TM_RISINGEDGE 0x04u
#define Async_Feedback_Counter__B_TIMER__TM_FALLINGEDGE 0x08u
#define Async_Feedback_Counter__B_TIMER__TM_EITHEREDGE 0x0Cu
#define Async_Feedback_Counter__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define Async_Feedback_Counter_INIT_PERIOD             255u
#define Async_Feedback_Counter_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << Async_Feedback_Counter_CTRL_CAP_MODE_SHIFT))
#define Async_Feedback_Counter_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << Async_Feedback_Counter_CTRL_TRIG_MODE_SHIFT))
#if (Async_Feedback_Counter_UsingFixedFunction)
    #define Async_Feedback_Counter_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << Async_Feedback_Counter_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << Async_Feedback_Counter_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define Async_Feedback_Counter_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << Async_Feedback_Counter_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Async_Feedback_Counter_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Async_Feedback_Counter_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (Async_Feedback_Counter_UsingFixedFunction) */
#define Async_Feedback_Counter_INIT_CAPTURE_COUNT      (2u)
#define Async_Feedback_Counter_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << Async_Feedback_Counter_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (Async_Feedback_Counter_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define Async_Feedback_Counter_STATUS         (*(reg8 *) Async_Feedback_Counter_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Async_Feedback_Counter_STATUS_MASK    (*(reg8 *) Async_Feedback_Counter_TimerHW__SR0 )
    #define Async_Feedback_Counter_CONTROL        (*(reg8 *) Async_Feedback_Counter_TimerHW__CFG0)
    #define Async_Feedback_Counter_CONTROL2       (*(reg8 *) Async_Feedback_Counter_TimerHW__CFG1)
    #define Async_Feedback_Counter_CONTROL2_PTR   ( (reg8 *) Async_Feedback_Counter_TimerHW__CFG1)
    #define Async_Feedback_Counter_RT1            (*(reg8 *) Async_Feedback_Counter_TimerHW__RT1)
    #define Async_Feedback_Counter_RT1_PTR        ( (reg8 *) Async_Feedback_Counter_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define Async_Feedback_Counter_CONTROL3       (*(reg8 *) Async_Feedback_Counter_TimerHW__CFG2)
        #define Async_Feedback_Counter_CONTROL3_PTR   ( (reg8 *) Async_Feedback_Counter_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define Async_Feedback_Counter_GLOBAL_ENABLE  (*(reg8 *) Async_Feedback_Counter_TimerHW__PM_ACT_CFG)
    #define Async_Feedback_Counter_GLOBAL_STBY_ENABLE  (*(reg8 *) Async_Feedback_Counter_TimerHW__PM_STBY_CFG)

    #define Async_Feedback_Counter_CAPTURE_LSB         (* (reg16 *) Async_Feedback_Counter_TimerHW__CAP0 )
    #define Async_Feedback_Counter_CAPTURE_LSB_PTR       ((reg16 *) Async_Feedback_Counter_TimerHW__CAP0 )
    #define Async_Feedback_Counter_PERIOD_LSB          (* (reg16 *) Async_Feedback_Counter_TimerHW__PER0 )
    #define Async_Feedback_Counter_PERIOD_LSB_PTR        ((reg16 *) Async_Feedback_Counter_TimerHW__PER0 )
    #define Async_Feedback_Counter_COUNTER_LSB         (* (reg16 *) Async_Feedback_Counter_TimerHW__CNT_CMP0 )
    #define Async_Feedback_Counter_COUNTER_LSB_PTR       ((reg16 *) Async_Feedback_Counter_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define Async_Feedback_Counter_BLOCK_EN_MASK                     Async_Feedback_Counter_TimerHW__PM_ACT_MSK
    #define Async_Feedback_Counter_BLOCK_STBY_EN_MASK                Async_Feedback_Counter_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define Async_Feedback_Counter_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define Async_Feedback_Counter_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define Async_Feedback_Counter_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define Async_Feedback_Counter_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define Async_Feedback_Counter_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define Async_Feedback_Counter_CTRL_ENABLE                        ((uint8)((uint8)0x01u << Async_Feedback_Counter_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Async_Feedback_Counter_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define Async_Feedback_Counter_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << Async_Feedback_Counter_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define Async_Feedback_Counter_CTRL_MODE_SHIFT                 0x01u
        #define Async_Feedback_Counter_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << Async_Feedback_Counter_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define Async_Feedback_Counter_CTRL_RCOD_SHIFT        0x02u
        #define Async_Feedback_Counter_CTRL_ENBL_SHIFT        0x00u
        #define Async_Feedback_Counter_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define Async_Feedback_Counter_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << Async_Feedback_Counter_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define Async_Feedback_Counter_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << Async_Feedback_Counter_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define Async_Feedback_Counter_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << Async_Feedback_Counter_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define Async_Feedback_Counter_CTRL_RCOD       ((uint8)((uint8)0x03u << Async_Feedback_Counter_CTRL_RCOD_SHIFT))
        #define Async_Feedback_Counter_CTRL_ENBL       ((uint8)((uint8)0x80u << Async_Feedback_Counter_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define Async_Feedback_Counter_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define Async_Feedback_Counter_RT1_MASK                        ((uint8)((uint8)0x03u << Async_Feedback_Counter_RT1_SHIFT))
    #define Async_Feedback_Counter_SYNC                            ((uint8)((uint8)0x03u << Async_Feedback_Counter_RT1_SHIFT))
    #define Async_Feedback_Counter_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define Async_Feedback_Counter_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << Async_Feedback_Counter_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define Async_Feedback_Counter_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << Async_Feedback_Counter_SYNCDSI_SHIFT))

    #define Async_Feedback_Counter_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << Async_Feedback_Counter_CTRL_MODE_SHIFT))
    #define Async_Feedback_Counter_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << Async_Feedback_Counter_CTRL_MODE_SHIFT))
    #define Async_Feedback_Counter_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << Async_Feedback_Counter_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Async_Feedback_Counter_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Async_Feedback_Counter_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Async_Feedback_Counter_STATUS_TC_INT_MASK_SHIFT        (Async_Feedback_Counter_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Async_Feedback_Counter_STATUS_CAPTURE_INT_MASK_SHIFT   (Async_Feedback_Counter_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define Async_Feedback_Counter_STATUS_TC                       ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_TC_SHIFT))
    #define Async_Feedback_Counter_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define Async_Feedback_Counter_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define Async_Feedback_Counter_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define Async_Feedback_Counter_STATUS              (* (reg8 *) Async_Feedback_Counter_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define Async_Feedback_Counter_STATUS_MASK         (* (reg8 *) Async_Feedback_Counter_TimerUDB_rstSts_stsreg__MASK_REG)
    #define Async_Feedback_Counter_STATUS_AUX_CTRL     (* (reg8 *) Async_Feedback_Counter_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Async_Feedback_Counter_CONTROL             (* (reg8 *) Async_Feedback_Counter_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(Async_Feedback_Counter_Resolution <= 8u) /* 8-bit Timer */
        #define Async_Feedback_Counter_CAPTURE_LSB         (* (reg8 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Async_Feedback_Counter_CAPTURE_LSB_PTR       ((reg8 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Async_Feedback_Counter_PERIOD_LSB          (* (reg8 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Async_Feedback_Counter_PERIOD_LSB_PTR        ((reg8 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Async_Feedback_Counter_COUNTER_LSB         (* (reg8 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define Async_Feedback_Counter_COUNTER_LSB_PTR       ((reg8 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
    #elif(Async_Feedback_Counter_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define Async_Feedback_Counter_CAPTURE_LSB         (* (reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Async_Feedback_Counter_CAPTURE_LSB_PTR       ((reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB          (* (reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB_PTR        ((reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB         (* (reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB_PTR       ((reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define Async_Feedback_Counter_CAPTURE_LSB         (* (reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define Async_Feedback_Counter_CAPTURE_LSB_PTR       ((reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB          (* (reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB_PTR        ((reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB         (* (reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB_PTR       ((reg16 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(Async_Feedback_Counter_Resolution <= 24u)/* 24-bit Timer */
        #define Async_Feedback_Counter_CAPTURE_LSB         (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Async_Feedback_Counter_CAPTURE_LSB_PTR       ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Async_Feedback_Counter_PERIOD_LSB          (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Async_Feedback_Counter_PERIOD_LSB_PTR        ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Async_Feedback_Counter_COUNTER_LSB         (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define Async_Feedback_Counter_COUNTER_LSB_PTR       ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define Async_Feedback_Counter_CAPTURE_LSB         (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Async_Feedback_Counter_CAPTURE_LSB_PTR       ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB          (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB_PTR        ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB         (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB_PTR       ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define Async_Feedback_Counter_CAPTURE_LSB         (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define Async_Feedback_Counter_CAPTURE_LSB_PTR       ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB          (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define Async_Feedback_Counter_PERIOD_LSB_PTR        ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB         (* (reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
            #define Async_Feedback_Counter_COUNTER_LSB_PTR       ((reg32 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define Async_Feedback_Counter_COUNTER_LSB_PTR_8BIT       ((reg8 *) Async_Feedback_Counter_TimerUDB_sT8_timerdp_u0__A0_REG )
    
    #if (Async_Feedback_Counter_UsingHWCaptureCounter)
        #define Async_Feedback_Counter_CAP_COUNT              (*(reg8 *) Async_Feedback_Counter_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Async_Feedback_Counter_CAP_COUNT_PTR          ( (reg8 *) Async_Feedback_Counter_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Async_Feedback_Counter_CAPTURE_COUNT_CTRL     (*(reg8 *) Async_Feedback_Counter_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define Async_Feedback_Counter_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) Async_Feedback_Counter_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (Async_Feedback_Counter_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define Async_Feedback_Counter_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define Async_Feedback_Counter_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << Async_Feedback_Counter_CTRL_INTCNT_SHIFT))
    #define Async_Feedback_Counter_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << Async_Feedback_Counter_CTRL_TRIG_MODE_SHIFT))
    #define Async_Feedback_Counter_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << Async_Feedback_Counter_CTRL_TRIG_EN_SHIFT))
    #define Async_Feedback_Counter_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << Async_Feedback_Counter_CTRL_CAP_MODE_SHIFT))
    #define Async_Feedback_Counter_CTRL_ENABLE                    ((uint8)((uint8)0x01u << Async_Feedback_Counter_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define Async_Feedback_Counter_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define Async_Feedback_Counter_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_STATUS_TC_INT_MASK_SHIFT       Async_Feedback_Counter_STATUS_TC_SHIFT
    #define Async_Feedback_Counter_STATUS_CAPTURE_INT_MASK_SHIFT  Async_Feedback_Counter_STATUS_CAPTURE_SHIFT
    #define Async_Feedback_Counter_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define Async_Feedback_Counter_STATUS_FIFOFULL_INT_MASK_SHIFT Async_Feedback_Counter_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define Async_Feedback_Counter_STATUS_TC                      ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define Async_Feedback_Counter_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Async_Feedback_Counter_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Async_Feedback_Counter_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define Async_Feedback_Counter_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define Async_Feedback_Counter_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Async_Feedback_Counter_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << Async_Feedback_Counter_STATUS_FIFOFULL_SHIFT))

    #define Async_Feedback_Counter_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define Async_Feedback_Counter_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define Async_Feedback_Counter_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define Async_Feedback_Counter_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define Async_Feedback_Counter_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define Async_Feedback_Counter_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_Async_Feedback_Counter_H */


/* [] END OF FILE */
