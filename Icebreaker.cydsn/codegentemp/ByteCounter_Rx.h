/*******************************************************************************
* File Name: ByteCounter_Rx.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the ByteCounter_Rx
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_TCPWM_ByteCounter_Rx_H)
#define CY_TCPWM_ByteCounter_Rx_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} ByteCounter_Rx_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  ByteCounter_Rx_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define ByteCounter_Rx_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define ByteCounter_Rx_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define ByteCounter_Rx_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define ByteCounter_Rx_QUAD_ENCODING_MODES            (0lu)
#define ByteCounter_Rx_QUAD_AUTO_START                (0lu)

/* Signal modes */
#define ByteCounter_Rx_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define ByteCounter_Rx_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define ByteCounter_Rx_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define ByteCounter_Rx_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define ByteCounter_Rx_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define ByteCounter_Rx_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define ByteCounter_Rx_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define ByteCounter_Rx_TC_RUN_MODE                    (0lu)
#define ByteCounter_Rx_TC_COUNTER_MODE                (0lu)
#define ByteCounter_Rx_TC_COMP_CAP_MODE               (2lu)
#define ByteCounter_Rx_TC_PRESCALER                   (0lu)

/* Signal modes */
#define ByteCounter_Rx_TC_RELOAD_SIGNAL_MODE          (0lu)
#define ByteCounter_Rx_TC_COUNT_SIGNAL_MODE           (0lu)
#define ByteCounter_Rx_TC_START_SIGNAL_MODE           (0lu)
#define ByteCounter_Rx_TC_STOP_SIGNAL_MODE            (0lu)
#define ByteCounter_Rx_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define ByteCounter_Rx_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define ByteCounter_Rx_TC_COUNT_SIGNAL_PRESENT        (1lu)
#define ByteCounter_Rx_TC_START_SIGNAL_PRESENT        (0lu)
#define ByteCounter_Rx_TC_STOP_SIGNAL_PRESENT         (0lu)
#define ByteCounter_Rx_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define ByteCounter_Rx_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define ByteCounter_Rx_PWM_KILL_EVENT                 (0lu)
#define ByteCounter_Rx_PWM_STOP_EVENT                 (0lu)
#define ByteCounter_Rx_PWM_MODE                       (4lu)
#define ByteCounter_Rx_PWM_OUT_N_INVERT               (0lu)
#define ByteCounter_Rx_PWM_OUT_INVERT                 (0lu)
#define ByteCounter_Rx_PWM_ALIGN                      (0lu)
#define ByteCounter_Rx_PWM_RUN_MODE                   (0lu)
#define ByteCounter_Rx_PWM_DEAD_TIME_CYCLE            (0lu)
#define ByteCounter_Rx_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define ByteCounter_Rx_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define ByteCounter_Rx_PWM_COUNT_SIGNAL_MODE          (3lu)
#define ByteCounter_Rx_PWM_START_SIGNAL_MODE          (0lu)
#define ByteCounter_Rx_PWM_STOP_SIGNAL_MODE           (0lu)
#define ByteCounter_Rx_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define ByteCounter_Rx_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define ByteCounter_Rx_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define ByteCounter_Rx_PWM_START_SIGNAL_PRESENT       (0lu)
#define ByteCounter_Rx_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define ByteCounter_Rx_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define ByteCounter_Rx_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define ByteCounter_Rx_TC_PERIOD_VALUE                (143lu)
#define ByteCounter_Rx_TC_COMPARE_VALUE               (65535lu)
#define ByteCounter_Rx_TC_COMPARE_BUF_VALUE           (65535lu)
#define ByteCounter_Rx_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define ByteCounter_Rx_PWM_PERIOD_VALUE               (65535lu)
#define ByteCounter_Rx_PWM_PERIOD_BUF_VALUE           (65535lu)
#define ByteCounter_Rx_PWM_PERIOD_SWAP                (0lu)
#define ByteCounter_Rx_PWM_COMPARE_VALUE              (65535lu)
#define ByteCounter_Rx_PWM_COMPARE_BUF_VALUE          (65535lu)
#define ByteCounter_Rx_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define ByteCounter_Rx__LEFT 0
#define ByteCounter_Rx__RIGHT 1
#define ByteCounter_Rx__CENTER 2
#define ByteCounter_Rx__ASYMMETRIC 3

#define ByteCounter_Rx__X1 0
#define ByteCounter_Rx__X2 1
#define ByteCounter_Rx__X4 2

#define ByteCounter_Rx__PWM 4
#define ByteCounter_Rx__PWM_DT 5
#define ByteCounter_Rx__PWM_PR 6

#define ByteCounter_Rx__INVERSE 1
#define ByteCounter_Rx__DIRECT 0

#define ByteCounter_Rx__CAPTURE 2
#define ByteCounter_Rx__COMPARE 0

#define ByteCounter_Rx__TRIG_LEVEL 3
#define ByteCounter_Rx__TRIG_RISING 0
#define ByteCounter_Rx__TRIG_FALLING 1
#define ByteCounter_Rx__TRIG_BOTH 2

#define ByteCounter_Rx__INTR_MASK_TC 1
#define ByteCounter_Rx__INTR_MASK_CC_MATCH 2
#define ByteCounter_Rx__INTR_MASK_NONE 0
#define ByteCounter_Rx__INTR_MASK_TC_CC 3

#define ByteCounter_Rx__UNCONFIG 8
#define ByteCounter_Rx__TIMER 1
#define ByteCounter_Rx__QUAD 3
#define ByteCounter_Rx__PWM_SEL 7

#define ByteCounter_Rx__COUNT_UP 0
#define ByteCounter_Rx__COUNT_DOWN 1
#define ByteCounter_Rx__COUNT_UPDOWN0 2
#define ByteCounter_Rx__COUNT_UPDOWN1 3


/* Prescaler */
#define ByteCounter_Rx_PRESCALE_DIVBY1                ((uint32)(0u << ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_PRESCALE_DIVBY2                ((uint32)(1u << ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_PRESCALE_DIVBY4                ((uint32)(2u << ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_PRESCALE_DIVBY8                ((uint32)(3u << ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_PRESCALE_DIVBY16               ((uint32)(4u << ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_PRESCALE_DIVBY32               ((uint32)(5u << ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_PRESCALE_DIVBY64               ((uint32)(6u << ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_PRESCALE_DIVBY128              ((uint32)(7u << ByteCounter_Rx_PRESCALER_SHIFT))

/* TCPWM set modes */
#define ByteCounter_Rx_MODE_TIMER_COMPARE             ((uint32)(ByteCounter_Rx__COMPARE         <<  \
                                                                  ByteCounter_Rx_MODE_SHIFT))
#define ByteCounter_Rx_MODE_TIMER_CAPTURE             ((uint32)(ByteCounter_Rx__CAPTURE         <<  \
                                                                  ByteCounter_Rx_MODE_SHIFT))
#define ByteCounter_Rx_MODE_QUAD                      ((uint32)(ByteCounter_Rx__QUAD            <<  \
                                                                  ByteCounter_Rx_MODE_SHIFT))
#define ByteCounter_Rx_MODE_PWM                       ((uint32)(ByteCounter_Rx__PWM             <<  \
                                                                  ByteCounter_Rx_MODE_SHIFT))
#define ByteCounter_Rx_MODE_PWM_DT                    ((uint32)(ByteCounter_Rx__PWM_DT          <<  \
                                                                  ByteCounter_Rx_MODE_SHIFT))
#define ByteCounter_Rx_MODE_PWM_PR                    ((uint32)(ByteCounter_Rx__PWM_PR          <<  \
                                                                  ByteCounter_Rx_MODE_SHIFT))

/* Quad Modes */
#define ByteCounter_Rx_MODE_X1                        ((uint32)(ByteCounter_Rx__X1              <<  \
                                                                  ByteCounter_Rx_QUAD_MODE_SHIFT))
#define ByteCounter_Rx_MODE_X2                        ((uint32)(ByteCounter_Rx__X2              <<  \
                                                                  ByteCounter_Rx_QUAD_MODE_SHIFT))
#define ByteCounter_Rx_MODE_X4                        ((uint32)(ByteCounter_Rx__X4              <<  \
                                                                  ByteCounter_Rx_QUAD_MODE_SHIFT))

/* Counter modes */
#define ByteCounter_Rx_COUNT_UP                       ((uint32)(ByteCounter_Rx__COUNT_UP        <<  \
                                                                  ByteCounter_Rx_UPDOWN_SHIFT))
#define ByteCounter_Rx_COUNT_DOWN                     ((uint32)(ByteCounter_Rx__COUNT_DOWN      <<  \
                                                                  ByteCounter_Rx_UPDOWN_SHIFT))
#define ByteCounter_Rx_COUNT_UPDOWN0                  ((uint32)(ByteCounter_Rx__COUNT_UPDOWN0   <<  \
                                                                  ByteCounter_Rx_UPDOWN_SHIFT))
#define ByteCounter_Rx_COUNT_UPDOWN1                  ((uint32)(ByteCounter_Rx__COUNT_UPDOWN1   <<  \
                                                                  ByteCounter_Rx_UPDOWN_SHIFT))

/* PWM output invert */
#define ByteCounter_Rx_INVERT_LINE                    ((uint32)(ByteCounter_Rx__INVERSE         <<  \
                                                                  ByteCounter_Rx_INV_OUT_SHIFT))
#define ByteCounter_Rx_INVERT_LINE_N                  ((uint32)(ByteCounter_Rx__INVERSE         <<  \
                                                                  ByteCounter_Rx_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define ByteCounter_Rx_TRIG_RISING                    ((uint32)ByteCounter_Rx__TRIG_RISING)
#define ByteCounter_Rx_TRIG_FALLING                   ((uint32)ByteCounter_Rx__TRIG_FALLING)
#define ByteCounter_Rx_TRIG_BOTH                      ((uint32)ByteCounter_Rx__TRIG_BOTH)
#define ByteCounter_Rx_TRIG_LEVEL                     ((uint32)ByteCounter_Rx__TRIG_LEVEL)

/* Interrupt mask */
#define ByteCounter_Rx_INTR_MASK_TC                   ((uint32)ByteCounter_Rx__INTR_MASK_TC)
#define ByteCounter_Rx_INTR_MASK_CC_MATCH             ((uint32)ByteCounter_Rx__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define ByteCounter_Rx_CC_MATCH_SET                   (0x00u)
#define ByteCounter_Rx_CC_MATCH_CLEAR                 (0x01u)
#define ByteCounter_Rx_CC_MATCH_INVERT                (0x02u)
#define ByteCounter_Rx_CC_MATCH_NO_CHANGE             (0x03u)
#define ByteCounter_Rx_OVERLOW_SET                    (0x00u)
#define ByteCounter_Rx_OVERLOW_CLEAR                  (0x04u)
#define ByteCounter_Rx_OVERLOW_INVERT                 (0x08u)
#define ByteCounter_Rx_OVERLOW_NO_CHANGE              (0x0Cu)
#define ByteCounter_Rx_UNDERFLOW_SET                  (0x00u)
#define ByteCounter_Rx_UNDERFLOW_CLEAR                (0x10u)
#define ByteCounter_Rx_UNDERFLOW_INVERT               (0x20u)
#define ByteCounter_Rx_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define ByteCounter_Rx_PWM_MODE_LEFT                  (ByteCounter_Rx_CC_MATCH_CLEAR        |   \
                                                         ByteCounter_Rx_OVERLOW_SET           |   \
                                                         ByteCounter_Rx_UNDERFLOW_NO_CHANGE)
#define ByteCounter_Rx_PWM_MODE_RIGHT                 (ByteCounter_Rx_CC_MATCH_SET          |   \
                                                         ByteCounter_Rx_OVERLOW_NO_CHANGE     |   \
                                                         ByteCounter_Rx_UNDERFLOW_CLEAR)
#define ByteCounter_Rx_PWM_MODE_ASYM                  (ByteCounter_Rx_CC_MATCH_INVERT       |   \
                                                         ByteCounter_Rx_OVERLOW_SET           |   \
                                                         ByteCounter_Rx_UNDERFLOW_CLEAR)

#if (ByteCounter_Rx_CY_TCPWM_V2)
    #if(ByteCounter_Rx_CY_TCPWM_4000)
        #define ByteCounter_Rx_PWM_MODE_CENTER                (ByteCounter_Rx_CC_MATCH_INVERT       |   \
                                                                 ByteCounter_Rx_OVERLOW_NO_CHANGE     |   \
                                                                 ByteCounter_Rx_UNDERFLOW_CLEAR)
    #else
        #define ByteCounter_Rx_PWM_MODE_CENTER                (ByteCounter_Rx_CC_MATCH_INVERT       |   \
                                                                 ByteCounter_Rx_OVERLOW_SET           |   \
                                                                 ByteCounter_Rx_UNDERFLOW_CLEAR)
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */
#else
    #define ByteCounter_Rx_PWM_MODE_CENTER                (ByteCounter_Rx_CC_MATCH_INVERT       |   \
                                                             ByteCounter_Rx_OVERLOW_NO_CHANGE     |   \
                                                             ByteCounter_Rx_UNDERFLOW_CLEAR)
#endif /* (ByteCounter_Rx_CY_TCPWM_NEW) */

/* Command operations without condition */
#define ByteCounter_Rx_CMD_CAPTURE                    (0u)
#define ByteCounter_Rx_CMD_RELOAD                     (8u)
#define ByteCounter_Rx_CMD_STOP                       (16u)
#define ByteCounter_Rx_CMD_START                      (24u)

/* Status */
#define ByteCounter_Rx_STATUS_DOWN                    (1u)
#define ByteCounter_Rx_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   ByteCounter_Rx_Init(void);
void   ByteCounter_Rx_Enable(void);
void   ByteCounter_Rx_Start(void);
void   ByteCounter_Rx_Stop(void);

void   ByteCounter_Rx_SetMode(uint32 mode);
void   ByteCounter_Rx_SetCounterMode(uint32 counterMode);
void   ByteCounter_Rx_SetPWMMode(uint32 modeMask);
void   ByteCounter_Rx_SetQDMode(uint32 qdMode);

void   ByteCounter_Rx_SetPrescaler(uint32 prescaler);
void   ByteCounter_Rx_TriggerCommand(uint32 mask, uint32 command);
void   ByteCounter_Rx_SetOneShot(uint32 oneShotEnable);
uint32 ByteCounter_Rx_ReadStatus(void);

void   ByteCounter_Rx_SetPWMSyncKill(uint32 syncKillEnable);
void   ByteCounter_Rx_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   ByteCounter_Rx_SetPWMDeadTime(uint32 deadTime);
void   ByteCounter_Rx_SetPWMInvert(uint32 mask);

void   ByteCounter_Rx_SetInterruptMode(uint32 interruptMask);
uint32 ByteCounter_Rx_GetInterruptSourceMasked(void);
uint32 ByteCounter_Rx_GetInterruptSource(void);
void   ByteCounter_Rx_ClearInterrupt(uint32 interruptMask);
void   ByteCounter_Rx_SetInterrupt(uint32 interruptMask);

void   ByteCounter_Rx_WriteCounter(uint32 count);
uint32 ByteCounter_Rx_ReadCounter(void);

uint32 ByteCounter_Rx_ReadCapture(void);
uint32 ByteCounter_Rx_ReadCaptureBuf(void);

void   ByteCounter_Rx_WritePeriod(uint32 period);
uint32 ByteCounter_Rx_ReadPeriod(void);
void   ByteCounter_Rx_WritePeriodBuf(uint32 periodBuf);
uint32 ByteCounter_Rx_ReadPeriodBuf(void);

void   ByteCounter_Rx_WriteCompare(uint32 compare);
uint32 ByteCounter_Rx_ReadCompare(void);
void   ByteCounter_Rx_WriteCompareBuf(uint32 compareBuf);
uint32 ByteCounter_Rx_ReadCompareBuf(void);

void   ByteCounter_Rx_SetPeriodSwap(uint32 swapEnable);
void   ByteCounter_Rx_SetCompareSwap(uint32 swapEnable);

void   ByteCounter_Rx_SetCaptureMode(uint32 triggerMode);
void   ByteCounter_Rx_SetReloadMode(uint32 triggerMode);
void   ByteCounter_Rx_SetStartMode(uint32 triggerMode);
void   ByteCounter_Rx_SetStopMode(uint32 triggerMode);
void   ByteCounter_Rx_SetCountMode(uint32 triggerMode);

void   ByteCounter_Rx_SaveConfig(void);
void   ByteCounter_Rx_RestoreConfig(void);
void   ByteCounter_Rx_Sleep(void);
void   ByteCounter_Rx_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define ByteCounter_Rx_BLOCK_CONTROL_REG              (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define ByteCounter_Rx_BLOCK_CONTROL_PTR              ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define ByteCounter_Rx_COMMAND_REG                    (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define ByteCounter_Rx_COMMAND_PTR                    ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define ByteCounter_Rx_INTRRUPT_CAUSE_REG             (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define ByteCounter_Rx_INTRRUPT_CAUSE_PTR             ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define ByteCounter_Rx_CONTROL_REG                    (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__CTRL )
#define ByteCounter_Rx_CONTROL_PTR                    ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__CTRL )
#define ByteCounter_Rx_STATUS_REG                     (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__STATUS )
#define ByteCounter_Rx_STATUS_PTR                     ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__STATUS )
#define ByteCounter_Rx_COUNTER_REG                    (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__COUNTER )
#define ByteCounter_Rx_COUNTER_PTR                    ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__COUNTER )
#define ByteCounter_Rx_COMP_CAP_REG                   (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__CC )
#define ByteCounter_Rx_COMP_CAP_PTR                   ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__CC )
#define ByteCounter_Rx_COMP_CAP_BUF_REG               (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__CC_BUFF )
#define ByteCounter_Rx_COMP_CAP_BUF_PTR               ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__CC_BUFF )
#define ByteCounter_Rx_PERIOD_REG                     (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__PERIOD )
#define ByteCounter_Rx_PERIOD_PTR                     ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__PERIOD )
#define ByteCounter_Rx_PERIOD_BUF_REG                 (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define ByteCounter_Rx_PERIOD_BUF_PTR                 ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define ByteCounter_Rx_TRIG_CONTROL0_REG              (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define ByteCounter_Rx_TRIG_CONTROL0_PTR              ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define ByteCounter_Rx_TRIG_CONTROL1_REG              (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define ByteCounter_Rx_TRIG_CONTROL1_PTR              ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define ByteCounter_Rx_TRIG_CONTROL2_REG              (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define ByteCounter_Rx_TRIG_CONTROL2_PTR              ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define ByteCounter_Rx_INTERRUPT_REQ_REG              (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR )
#define ByteCounter_Rx_INTERRUPT_REQ_PTR              ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR )
#define ByteCounter_Rx_INTERRUPT_SET_REG              (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR_SET )
#define ByteCounter_Rx_INTERRUPT_SET_PTR              ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR_SET )
#define ByteCounter_Rx_INTERRUPT_MASK_REG             (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR_MASK )
#define ByteCounter_Rx_INTERRUPT_MASK_PTR             ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR_MASK )
#define ByteCounter_Rx_INTERRUPT_MASKED_REG           (*(reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR_MASKED )
#define ByteCounter_Rx_INTERRUPT_MASKED_PTR           ( (reg32 *) ByteCounter_Rx_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define ByteCounter_Rx_MASK                           ((uint32)ByteCounter_Rx_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define ByteCounter_Rx_RELOAD_CC_SHIFT                (0u)
#define ByteCounter_Rx_RELOAD_PERIOD_SHIFT            (1u)
#define ByteCounter_Rx_PWM_SYNC_KILL_SHIFT            (2u)
#define ByteCounter_Rx_PWM_STOP_KILL_SHIFT            (3u)
#define ByteCounter_Rx_PRESCALER_SHIFT                (8u)
#define ByteCounter_Rx_UPDOWN_SHIFT                   (16u)
#define ByteCounter_Rx_ONESHOT_SHIFT                  (18u)
#define ByteCounter_Rx_QUAD_MODE_SHIFT                (20u)
#define ByteCounter_Rx_INV_OUT_SHIFT                  (20u)
#define ByteCounter_Rx_INV_COMPL_OUT_SHIFT            (21u)
#define ByteCounter_Rx_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define ByteCounter_Rx_RELOAD_CC_MASK                 ((uint32)(ByteCounter_Rx_1BIT_MASK        <<  \
                                                                            ByteCounter_Rx_RELOAD_CC_SHIFT))
#define ByteCounter_Rx_RELOAD_PERIOD_MASK             ((uint32)(ByteCounter_Rx_1BIT_MASK        <<  \
                                                                            ByteCounter_Rx_RELOAD_PERIOD_SHIFT))
#define ByteCounter_Rx_PWM_SYNC_KILL_MASK             ((uint32)(ByteCounter_Rx_1BIT_MASK        <<  \
                                                                            ByteCounter_Rx_PWM_SYNC_KILL_SHIFT))
#define ByteCounter_Rx_PWM_STOP_KILL_MASK             ((uint32)(ByteCounter_Rx_1BIT_MASK        <<  \
                                                                            ByteCounter_Rx_PWM_STOP_KILL_SHIFT))
#define ByteCounter_Rx_PRESCALER_MASK                 ((uint32)(ByteCounter_Rx_8BIT_MASK        <<  \
                                                                            ByteCounter_Rx_PRESCALER_SHIFT))
#define ByteCounter_Rx_UPDOWN_MASK                    ((uint32)(ByteCounter_Rx_2BIT_MASK        <<  \
                                                                            ByteCounter_Rx_UPDOWN_SHIFT))
#define ByteCounter_Rx_ONESHOT_MASK                   ((uint32)(ByteCounter_Rx_1BIT_MASK        <<  \
                                                                            ByteCounter_Rx_ONESHOT_SHIFT))
#define ByteCounter_Rx_QUAD_MODE_MASK                 ((uint32)(ByteCounter_Rx_3BIT_MASK        <<  \
                                                                            ByteCounter_Rx_QUAD_MODE_SHIFT))
#define ByteCounter_Rx_INV_OUT_MASK                   ((uint32)(ByteCounter_Rx_2BIT_MASK        <<  \
                                                                            ByteCounter_Rx_INV_OUT_SHIFT))
#define ByteCounter_Rx_MODE_MASK                      ((uint32)(ByteCounter_Rx_3BIT_MASK        <<  \
                                                                            ByteCounter_Rx_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define ByteCounter_Rx_CAPTURE_SHIFT                  (0u)
#define ByteCounter_Rx_COUNT_SHIFT                    (2u)
#define ByteCounter_Rx_RELOAD_SHIFT                   (4u)
#define ByteCounter_Rx_STOP_SHIFT                     (6u)
#define ByteCounter_Rx_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define ByteCounter_Rx_CAPTURE_MASK                   ((uint32)(ByteCounter_Rx_2BIT_MASK        <<  \
                                                                  ByteCounter_Rx_CAPTURE_SHIFT))
#define ByteCounter_Rx_COUNT_MASK                     ((uint32)(ByteCounter_Rx_2BIT_MASK        <<  \
                                                                  ByteCounter_Rx_COUNT_SHIFT))
#define ByteCounter_Rx_RELOAD_MASK                    ((uint32)(ByteCounter_Rx_2BIT_MASK        <<  \
                                                                  ByteCounter_Rx_RELOAD_SHIFT))
#define ByteCounter_Rx_STOP_MASK                      ((uint32)(ByteCounter_Rx_2BIT_MASK        <<  \
                                                                  ByteCounter_Rx_STOP_SHIFT))
#define ByteCounter_Rx_START_MASK                     ((uint32)(ByteCounter_Rx_2BIT_MASK        <<  \
                                                                  ByteCounter_Rx_START_SHIFT))

/* MASK */
#define ByteCounter_Rx_1BIT_MASK                      ((uint32)0x01u)
#define ByteCounter_Rx_2BIT_MASK                      ((uint32)0x03u)
#define ByteCounter_Rx_3BIT_MASK                      ((uint32)0x07u)
#define ByteCounter_Rx_6BIT_MASK                      ((uint32)0x3Fu)
#define ByteCounter_Rx_8BIT_MASK                      ((uint32)0xFFu)
#define ByteCounter_Rx_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define ByteCounter_Rx_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define ByteCounter_Rx_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(ByteCounter_Rx_QUAD_ENCODING_MODES     << ByteCounter_Rx_QUAD_MODE_SHIFT))       |\
         ((uint32)(ByteCounter_Rx_CONFIG                  << ByteCounter_Rx_MODE_SHIFT)))

#define ByteCounter_Rx_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(ByteCounter_Rx_PWM_STOP_EVENT          << ByteCounter_Rx_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(ByteCounter_Rx_PWM_OUT_INVERT          << ByteCounter_Rx_INV_OUT_SHIFT))         |\
         ((uint32)(ByteCounter_Rx_PWM_OUT_N_INVERT        << ByteCounter_Rx_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(ByteCounter_Rx_PWM_MODE                << ByteCounter_Rx_MODE_SHIFT)))

#define ByteCounter_Rx_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(ByteCounter_Rx_PWM_RUN_MODE         << ByteCounter_Rx_ONESHOT_SHIFT))
            
#define ByteCounter_Rx_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(ByteCounter_Rx_PWM_ALIGN            << ByteCounter_Rx_UPDOWN_SHIFT))

#define ByteCounter_Rx_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(ByteCounter_Rx_PWM_KILL_EVENT      << ByteCounter_Rx_PWM_SYNC_KILL_SHIFT))

#define ByteCounter_Rx_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(ByteCounter_Rx_PWM_DEAD_TIME_CYCLE  << ByteCounter_Rx_PRESCALER_SHIFT))

#define ByteCounter_Rx_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(ByteCounter_Rx_PWM_PRESCALER        << ByteCounter_Rx_PRESCALER_SHIFT))

#define ByteCounter_Rx_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(ByteCounter_Rx_TC_PRESCALER            << ByteCounter_Rx_PRESCALER_SHIFT))       |\
         ((uint32)(ByteCounter_Rx_TC_COUNTER_MODE         << ByteCounter_Rx_UPDOWN_SHIFT))          |\
         ((uint32)(ByteCounter_Rx_TC_RUN_MODE             << ByteCounter_Rx_ONESHOT_SHIFT))         |\
         ((uint32)(ByteCounter_Rx_TC_COMP_CAP_MODE        << ByteCounter_Rx_MODE_SHIFT)))
        
#define ByteCounter_Rx_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(ByteCounter_Rx_QUAD_PHIA_SIGNAL_MODE   << ByteCounter_Rx_COUNT_SHIFT))           |\
         ((uint32)(ByteCounter_Rx_QUAD_INDEX_SIGNAL_MODE  << ByteCounter_Rx_RELOAD_SHIFT))          |\
         ((uint32)(ByteCounter_Rx_QUAD_STOP_SIGNAL_MODE   << ByteCounter_Rx_STOP_SHIFT))            |\
         ((uint32)(ByteCounter_Rx_QUAD_PHIB_SIGNAL_MODE   << ByteCounter_Rx_START_SHIFT)))

#define ByteCounter_Rx_PWM_SIGNALS_MODES                                                              \
        (((uint32)(ByteCounter_Rx_PWM_SWITCH_SIGNAL_MODE  << ByteCounter_Rx_CAPTURE_SHIFT))         |\
         ((uint32)(ByteCounter_Rx_PWM_COUNT_SIGNAL_MODE   << ByteCounter_Rx_COUNT_SHIFT))           |\
         ((uint32)(ByteCounter_Rx_PWM_RELOAD_SIGNAL_MODE  << ByteCounter_Rx_RELOAD_SHIFT))          |\
         ((uint32)(ByteCounter_Rx_PWM_STOP_SIGNAL_MODE    << ByteCounter_Rx_STOP_SHIFT))            |\
         ((uint32)(ByteCounter_Rx_PWM_START_SIGNAL_MODE   << ByteCounter_Rx_START_SHIFT)))

#define ByteCounter_Rx_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(ByteCounter_Rx_TC_CAPTURE_SIGNAL_MODE  << ByteCounter_Rx_CAPTURE_SHIFT))         |\
         ((uint32)(ByteCounter_Rx_TC_COUNT_SIGNAL_MODE    << ByteCounter_Rx_COUNT_SHIFT))           |\
         ((uint32)(ByteCounter_Rx_TC_RELOAD_SIGNAL_MODE   << ByteCounter_Rx_RELOAD_SHIFT))          |\
         ((uint32)(ByteCounter_Rx_TC_STOP_SIGNAL_MODE     << ByteCounter_Rx_STOP_SHIFT))            |\
         ((uint32)(ByteCounter_Rx_TC_START_SIGNAL_MODE    << ByteCounter_Rx_START_SHIFT)))
        
#define ByteCounter_Rx_TIMER_UPDOWN_CNT_USED                                                          \
                ((ByteCounter_Rx__COUNT_UPDOWN0 == ByteCounter_Rx_TC_COUNTER_MODE)                  ||\
                 (ByteCounter_Rx__COUNT_UPDOWN1 == ByteCounter_Rx_TC_COUNTER_MODE))

#define ByteCounter_Rx_PWM_UPDOWN_CNT_USED                                                            \
                ((ByteCounter_Rx__CENTER == ByteCounter_Rx_PWM_ALIGN)                               ||\
                 (ByteCounter_Rx__ASYMMETRIC == ByteCounter_Rx_PWM_ALIGN))               
        
#define ByteCounter_Rx_PWM_PR_INIT_VALUE              (1u)
#define ByteCounter_Rx_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_ByteCounter_Rx_H */

/* [] END OF FILE */
