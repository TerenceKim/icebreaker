/*******************************************************************************
* File Name: ByteCounter_Rx.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the ByteCounter_Rx
*  component
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

#include "ByteCounter_Rx.h"

uint8 ByteCounter_Rx_initVar = 0u;


/*******************************************************************************
* Function Name: ByteCounter_Rx_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default ByteCounter_Rx configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (ByteCounter_Rx__QUAD == ByteCounter_Rx_CONFIG)
        ByteCounter_Rx_CONTROL_REG = ByteCounter_Rx_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        ByteCounter_Rx_TRIG_CONTROL1_REG  = ByteCounter_Rx_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        ByteCounter_Rx_SetInterruptMode(ByteCounter_Rx_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        ByteCounter_Rx_SetCounterMode(ByteCounter_Rx_COUNT_DOWN);
        ByteCounter_Rx_WritePeriod(ByteCounter_Rx_QUAD_PERIOD_INIT_VALUE);
        ByteCounter_Rx_WriteCounter(ByteCounter_Rx_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (ByteCounter_Rx__QUAD == ByteCounter_Rx_CONFIG) */

    #if (ByteCounter_Rx__TIMER == ByteCounter_Rx_CONFIG)
        ByteCounter_Rx_CONTROL_REG = ByteCounter_Rx_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        ByteCounter_Rx_TRIG_CONTROL1_REG  = ByteCounter_Rx_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        ByteCounter_Rx_SetInterruptMode(ByteCounter_Rx_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        ByteCounter_Rx_WritePeriod(ByteCounter_Rx_TC_PERIOD_VALUE );

        #if (ByteCounter_Rx__COMPARE == ByteCounter_Rx_TC_COMP_CAP_MODE)
            ByteCounter_Rx_WriteCompare(ByteCounter_Rx_TC_COMPARE_VALUE);

            #if (1u == ByteCounter_Rx_TC_COMPARE_SWAP)
                ByteCounter_Rx_SetCompareSwap(1u);
                ByteCounter_Rx_WriteCompareBuf(ByteCounter_Rx_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == ByteCounter_Rx_TC_COMPARE_SWAP) */
        #endif  /* (ByteCounter_Rx__COMPARE == ByteCounter_Rx_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (ByteCounter_Rx_CY_TCPWM_V2 && ByteCounter_Rx_TIMER_UPDOWN_CNT_USED && !ByteCounter_Rx_CY_TCPWM_4000)
            ByteCounter_Rx_WriteCounter(1u);
        #elif(ByteCounter_Rx__COUNT_DOWN == ByteCounter_Rx_TC_COUNTER_MODE)
            ByteCounter_Rx_WriteCounter(ByteCounter_Rx_TC_PERIOD_VALUE);
        #else
            ByteCounter_Rx_WriteCounter(0u);
        #endif /* (ByteCounter_Rx_CY_TCPWM_V2 && ByteCounter_Rx_TIMER_UPDOWN_CNT_USED && !ByteCounter_Rx_CY_TCPWM_4000) */
    #endif  /* (ByteCounter_Rx__TIMER == ByteCounter_Rx_CONFIG) */

    #if (ByteCounter_Rx__PWM_SEL == ByteCounter_Rx_CONFIG)
        ByteCounter_Rx_CONTROL_REG = ByteCounter_Rx_CTRL_PWM_BASE_CONFIG;

        #if (ByteCounter_Rx__PWM_PR == ByteCounter_Rx_PWM_MODE)
            ByteCounter_Rx_CONTROL_REG |= ByteCounter_Rx_CTRL_PWM_RUN_MODE;
            ByteCounter_Rx_WriteCounter(ByteCounter_Rx_PWM_PR_INIT_VALUE);
        #else
            ByteCounter_Rx_CONTROL_REG |= ByteCounter_Rx_CTRL_PWM_ALIGN | ByteCounter_Rx_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (ByteCounter_Rx_CY_TCPWM_V2 && ByteCounter_Rx_PWM_UPDOWN_CNT_USED && !ByteCounter_Rx_CY_TCPWM_4000)
                ByteCounter_Rx_WriteCounter(1u);
            #elif (ByteCounter_Rx__RIGHT == ByteCounter_Rx_PWM_ALIGN)
                ByteCounter_Rx_WriteCounter(ByteCounter_Rx_PWM_PERIOD_VALUE);
            #else 
                ByteCounter_Rx_WriteCounter(0u);
            #endif  /* (ByteCounter_Rx_CY_TCPWM_V2 && ByteCounter_Rx_PWM_UPDOWN_CNT_USED && !ByteCounter_Rx_CY_TCPWM_4000) */
        #endif  /* (ByteCounter_Rx__PWM_PR == ByteCounter_Rx_PWM_MODE) */

        #if (ByteCounter_Rx__PWM_DT == ByteCounter_Rx_PWM_MODE)
            ByteCounter_Rx_CONTROL_REG |= ByteCounter_Rx_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (ByteCounter_Rx__PWM_DT == ByteCounter_Rx_PWM_MODE) */

        #if (ByteCounter_Rx__PWM == ByteCounter_Rx_PWM_MODE)
            ByteCounter_Rx_CONTROL_REG |= ByteCounter_Rx_CTRL_PWM_PRESCALER;
        #endif  /* (ByteCounter_Rx__PWM == ByteCounter_Rx_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        ByteCounter_Rx_TRIG_CONTROL1_REG  = ByteCounter_Rx_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        ByteCounter_Rx_SetInterruptMode(ByteCounter_Rx_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (ByteCounter_Rx__PWM_PR == ByteCounter_Rx_PWM_MODE)
            ByteCounter_Rx_TRIG_CONTROL2_REG =
                    (ByteCounter_Rx_CC_MATCH_NO_CHANGE    |
                    ByteCounter_Rx_OVERLOW_NO_CHANGE      |
                    ByteCounter_Rx_UNDERFLOW_NO_CHANGE);
        #else
            #if (ByteCounter_Rx__LEFT == ByteCounter_Rx_PWM_ALIGN)
                ByteCounter_Rx_TRIG_CONTROL2_REG = ByteCounter_Rx_PWM_MODE_LEFT;
            #endif  /* ( ByteCounter_Rx_PWM_LEFT == ByteCounter_Rx_PWM_ALIGN) */

            #if (ByteCounter_Rx__RIGHT == ByteCounter_Rx_PWM_ALIGN)
                ByteCounter_Rx_TRIG_CONTROL2_REG = ByteCounter_Rx_PWM_MODE_RIGHT;
            #endif  /* ( ByteCounter_Rx_PWM_RIGHT == ByteCounter_Rx_PWM_ALIGN) */

            #if (ByteCounter_Rx__CENTER == ByteCounter_Rx_PWM_ALIGN)
                ByteCounter_Rx_TRIG_CONTROL2_REG = ByteCounter_Rx_PWM_MODE_CENTER;
            #endif  /* ( ByteCounter_Rx_PWM_CENTER == ByteCounter_Rx_PWM_ALIGN) */

            #if (ByteCounter_Rx__ASYMMETRIC == ByteCounter_Rx_PWM_ALIGN)
                ByteCounter_Rx_TRIG_CONTROL2_REG = ByteCounter_Rx_PWM_MODE_ASYM;
            #endif  /* (ByteCounter_Rx__ASYMMETRIC == ByteCounter_Rx_PWM_ALIGN) */
        #endif  /* (ByteCounter_Rx__PWM_PR == ByteCounter_Rx_PWM_MODE) */

        /* Set other values from customizer */
        ByteCounter_Rx_WritePeriod(ByteCounter_Rx_PWM_PERIOD_VALUE );
        ByteCounter_Rx_WriteCompare(ByteCounter_Rx_PWM_COMPARE_VALUE);

        #if (1u == ByteCounter_Rx_PWM_COMPARE_SWAP)
            ByteCounter_Rx_SetCompareSwap(1u);
            ByteCounter_Rx_WriteCompareBuf(ByteCounter_Rx_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == ByteCounter_Rx_PWM_COMPARE_SWAP) */

        #if (1u == ByteCounter_Rx_PWM_PERIOD_SWAP)
            ByteCounter_Rx_SetPeriodSwap(1u);
            ByteCounter_Rx_WritePeriodBuf(ByteCounter_Rx_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == ByteCounter_Rx_PWM_PERIOD_SWAP) */
    #endif  /* (ByteCounter_Rx__PWM_SEL == ByteCounter_Rx_CONFIG) */
    
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_Enable
********************************************************************************
*
* Summary:
*  Enables the ByteCounter_Rx.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    ByteCounter_Rx_BLOCK_CONTROL_REG |= ByteCounter_Rx_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (ByteCounter_Rx__PWM_SEL == ByteCounter_Rx_CONFIG)
        #if (0u == ByteCounter_Rx_PWM_START_SIGNAL_PRESENT)
            ByteCounter_Rx_TriggerCommand(ByteCounter_Rx_MASK, ByteCounter_Rx_CMD_START);
        #endif /* (0u == ByteCounter_Rx_PWM_START_SIGNAL_PRESENT) */
    #endif /* (ByteCounter_Rx__PWM_SEL == ByteCounter_Rx_CONFIG) */

    #if (ByteCounter_Rx__TIMER == ByteCounter_Rx_CONFIG)
        #if (0u == ByteCounter_Rx_TC_START_SIGNAL_PRESENT)
            ByteCounter_Rx_TriggerCommand(ByteCounter_Rx_MASK, ByteCounter_Rx_CMD_START);
        #endif /* (0u == ByteCounter_Rx_TC_START_SIGNAL_PRESENT) */
    #endif /* (ByteCounter_Rx__TIMER == ByteCounter_Rx_CONFIG) */
    
    #if (ByteCounter_Rx__QUAD == ByteCounter_Rx_CONFIG)
        #if (0u != ByteCounter_Rx_QUAD_AUTO_START)
            ByteCounter_Rx_TriggerCommand(ByteCounter_Rx_MASK, ByteCounter_Rx_CMD_RELOAD);
        #endif /* (0u != ByteCounter_Rx_QUAD_AUTO_START) */
    #endif  /* (ByteCounter_Rx__QUAD == ByteCounter_Rx_CONFIG) */
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_Start
********************************************************************************
*
* Summary:
*  Initializes the ByteCounter_Rx with default customizer
*  values when called the first time and enables the ByteCounter_Rx.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ByteCounter_Rx_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time ByteCounter_Rx_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the ByteCounter_Rx_Start() routine.
*
*******************************************************************************/
void ByteCounter_Rx_Start(void)
{
    if (0u == ByteCounter_Rx_initVar)
    {
        ByteCounter_Rx_Init();
        ByteCounter_Rx_initVar = 1u;
    }

    ByteCounter_Rx_Enable();
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_Stop
********************************************************************************
*
* Summary:
*  Disables the ByteCounter_Rx.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_BLOCK_CONTROL_REG &= (uint32)~ByteCounter_Rx_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the ByteCounter_Rx. This function is used when
*  configured as a generic ByteCounter_Rx and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the ByteCounter_Rx to operate in
*   Values:
*   - ByteCounter_Rx_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - ByteCounter_Rx_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - ByteCounter_Rx_MODE_QUAD - Quadrature decoder
*         - ByteCounter_Rx_MODE_PWM - PWM
*         - ByteCounter_Rx_MODE_PWM_DT - PWM with dead time
*         - ByteCounter_Rx_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_MODE_MASK;
    ByteCounter_Rx_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - ByteCounter_Rx_MODE_X1 - Counts on phi 1 rising
*         - ByteCounter_Rx_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - ByteCounter_Rx_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_QUAD_MODE_MASK;
    ByteCounter_Rx_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - ByteCounter_Rx_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - ByteCounter_Rx_PRESCALE_DIVBY2    - Divide by 2
*         - ByteCounter_Rx_PRESCALE_DIVBY4    - Divide by 4
*         - ByteCounter_Rx_PRESCALE_DIVBY8    - Divide by 8
*         - ByteCounter_Rx_PRESCALE_DIVBY16   - Divide by 16
*         - ByteCounter_Rx_PRESCALE_DIVBY32   - Divide by 32
*         - ByteCounter_Rx_PRESCALE_DIVBY64   - Divide by 64
*         - ByteCounter_Rx_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_PRESCALER_MASK;
    ByteCounter_Rx_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the ByteCounter_Rx runs
*  continuously or stops when terminal count is reached.  By default the
*  ByteCounter_Rx operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_ONESHOT_MASK;
    ByteCounter_Rx_CONTROL_REG |= ((uint32)((oneShotEnable & ByteCounter_Rx_1BIT_MASK) <<
                                                               ByteCounter_Rx_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetPWMMode(uint32 modeMask)
{
    ByteCounter_Rx_TRIG_CONTROL2_REG = (modeMask & ByteCounter_Rx_6BIT_MASK);
}



/*******************************************************************************
* Function Name: ByteCounter_Rx_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_PWM_SYNC_KILL_MASK;
    ByteCounter_Rx_CONTROL_REG |= ((uint32)((syncKillEnable & ByteCounter_Rx_1BIT_MASK)  <<
                                               ByteCounter_Rx_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_PWM_STOP_KILL_MASK;
    ByteCounter_Rx_CONTROL_REG |= ((uint32)((stopOnKillEnable & ByteCounter_Rx_1BIT_MASK)  <<
                                                         ByteCounter_Rx_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_PRESCALER_MASK;
    ByteCounter_Rx_CONTROL_REG |= ((uint32)((deadTime & ByteCounter_Rx_8BIT_MASK) <<
                                                          ByteCounter_Rx_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - ByteCounter_Rx_INVERT_LINE   - Inverts the line output
*         - ByteCounter_Rx_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_INV_OUT_MASK;
    ByteCounter_Rx_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: ByteCounter_Rx_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_WriteCounter(uint32 count)
{
    ByteCounter_Rx_COUNTER_REG = (count & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadCounter(void)
{
    return (ByteCounter_Rx_COUNTER_REG & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - ByteCounter_Rx_COUNT_UP       - Counts up
*     - ByteCounter_Rx_COUNT_DOWN     - Counts down
*     - ByteCounter_Rx_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - ByteCounter_Rx_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_UPDOWN_MASK;
    ByteCounter_Rx_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_WritePeriod(uint32 period)
{
    ByteCounter_Rx_PERIOD_REG = (period & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadPeriod(void)
{
    return (ByteCounter_Rx_PERIOD_REG & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_RELOAD_CC_MASK;
    ByteCounter_Rx_CONTROL_REG |= (swapEnable & ByteCounter_Rx_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_WritePeriodBuf(uint32 periodBuf)
{
    ByteCounter_Rx_PERIOD_BUF_REG = (periodBuf & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadPeriodBuf(void)
{
    return (ByteCounter_Rx_PERIOD_BUF_REG & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_CONTROL_REG &= (uint32)~ByteCounter_Rx_RELOAD_PERIOD_MASK;
    ByteCounter_Rx_CONTROL_REG |= ((uint32)((swapEnable & ByteCounter_Rx_1BIT_MASK) <<
                                                            ByteCounter_Rx_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void ByteCounter_Rx_WriteCompare(uint32 compare)
{
    #if (ByteCounter_Rx_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */

    #if (ByteCounter_Rx_CY_TCPWM_4000)
        currentMode = ((ByteCounter_Rx_CONTROL_REG & ByteCounter_Rx_UPDOWN_MASK) >> ByteCounter_Rx_UPDOWN_SHIFT);

        if (((uint32)ByteCounter_Rx__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)ByteCounter_Rx__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */
    
    ByteCounter_Rx_COMP_CAP_REG = (compare & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadCompare(void)
{
    #if (ByteCounter_Rx_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */

    #if (ByteCounter_Rx_CY_TCPWM_4000)
        currentMode = ((ByteCounter_Rx_CONTROL_REG & ByteCounter_Rx_UPDOWN_MASK) >> ByteCounter_Rx_UPDOWN_SHIFT);
        
        regVal = ByteCounter_Rx_COMP_CAP_REG;
        
        if (((uint32)ByteCounter_Rx__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)ByteCounter_Rx__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & ByteCounter_Rx_16BIT_MASK);
    #else
        return (ByteCounter_Rx_COMP_CAP_REG & ByteCounter_Rx_16BIT_MASK);
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void ByteCounter_Rx_WriteCompareBuf(uint32 compareBuf)
{
    #if (ByteCounter_Rx_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */

    #if (ByteCounter_Rx_CY_TCPWM_4000)
        currentMode = ((ByteCounter_Rx_CONTROL_REG & ByteCounter_Rx_UPDOWN_MASK) >> ByteCounter_Rx_UPDOWN_SHIFT);

        if (((uint32)ByteCounter_Rx__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)ByteCounter_Rx__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */
    
    ByteCounter_Rx_COMP_CAP_BUF_REG = (compareBuf & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadCompareBuf(void)
{
    #if (ByteCounter_Rx_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */

    #if (ByteCounter_Rx_CY_TCPWM_4000)
        currentMode = ((ByteCounter_Rx_CONTROL_REG & ByteCounter_Rx_UPDOWN_MASK) >> ByteCounter_Rx_UPDOWN_SHIFT);

        regVal = ByteCounter_Rx_COMP_CAP_BUF_REG;
        
        if (((uint32)ByteCounter_Rx__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)ByteCounter_Rx__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & ByteCounter_Rx_16BIT_MASK);
    #else
        return (ByteCounter_Rx_COMP_CAP_BUF_REG & ByteCounter_Rx_16BIT_MASK);
    #endif /* (ByteCounter_Rx_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadCapture(void)
{
    return (ByteCounter_Rx_COMP_CAP_REG & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadCaptureBuf(void)
{
    return (ByteCounter_Rx_COMP_CAP_BUF_REG & ByteCounter_Rx_16BIT_MASK);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - ByteCounter_Rx_TRIG_LEVEL     - Level
*     - ByteCounter_Rx_TRIG_RISING    - Rising edge
*     - ByteCounter_Rx_TRIG_FALLING   - Falling edge
*     - ByteCounter_Rx_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_TRIG_CONTROL1_REG &= (uint32)~ByteCounter_Rx_CAPTURE_MASK;
    ByteCounter_Rx_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - ByteCounter_Rx_TRIG_LEVEL     - Level
*     - ByteCounter_Rx_TRIG_RISING    - Rising edge
*     - ByteCounter_Rx_TRIG_FALLING   - Falling edge
*     - ByteCounter_Rx_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_TRIG_CONTROL1_REG &= (uint32)~ByteCounter_Rx_RELOAD_MASK;
    ByteCounter_Rx_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << ByteCounter_Rx_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - ByteCounter_Rx_TRIG_LEVEL     - Level
*     - ByteCounter_Rx_TRIG_RISING    - Rising edge
*     - ByteCounter_Rx_TRIG_FALLING   - Falling edge
*     - ByteCounter_Rx_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_TRIG_CONTROL1_REG &= (uint32)~ByteCounter_Rx_START_MASK;
    ByteCounter_Rx_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << ByteCounter_Rx_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - ByteCounter_Rx_TRIG_LEVEL     - Level
*     - ByteCounter_Rx_TRIG_RISING    - Rising edge
*     - ByteCounter_Rx_TRIG_FALLING   - Falling edge
*     - ByteCounter_Rx_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_TRIG_CONTROL1_REG &= (uint32)~ByteCounter_Rx_STOP_MASK;
    ByteCounter_Rx_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << ByteCounter_Rx_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - ByteCounter_Rx_TRIG_LEVEL     - Level
*     - ByteCounter_Rx_TRIG_RISING    - Rising edge
*     - ByteCounter_Rx_TRIG_FALLING   - Falling edge
*     - ByteCounter_Rx_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_TRIG_CONTROL1_REG &= (uint32)~ByteCounter_Rx_COUNT_MASK;
    ByteCounter_Rx_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << ByteCounter_Rx_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - ByteCounter_Rx_CMD_CAPTURE    - Trigger Capture/Switch command
*     - ByteCounter_Rx_CMD_RELOAD     - Trigger Reload/Index command
*     - ByteCounter_Rx_CMD_STOP       - Trigger Stop/Kill command
*     - ByteCounter_Rx_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    ByteCounter_Rx_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the ByteCounter_Rx.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - ByteCounter_Rx_STATUS_DOWN    - Set if counting down
*     - ByteCounter_Rx_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 ByteCounter_Rx_ReadStatus(void)
{
    return ((ByteCounter_Rx_STATUS_REG >> ByteCounter_Rx_RUNNING_STATUS_SHIFT) |
            (ByteCounter_Rx_STATUS_REG & ByteCounter_Rx_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - ByteCounter_Rx_INTR_MASK_TC       - Terminal count mask
*     - ByteCounter_Rx_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetInterruptMode(uint32 interruptMask)
{
    ByteCounter_Rx_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - ByteCounter_Rx_INTR_MASK_TC       - Terminal count mask
*     - ByteCounter_Rx_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 ByteCounter_Rx_GetInterruptSourceMasked(void)
{
    return (ByteCounter_Rx_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - ByteCounter_Rx_INTR_MASK_TC       - Terminal count mask
*     - ByteCounter_Rx_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 ByteCounter_Rx_GetInterruptSource(void)
{
    return (ByteCounter_Rx_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - ByteCounter_Rx_INTR_MASK_TC       - Terminal count mask
*     - ByteCounter_Rx_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_ClearInterrupt(uint32 interruptMask)
{
    ByteCounter_Rx_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: ByteCounter_Rx_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - ByteCounter_Rx_INTR_MASK_TC       - Terminal count mask
*     - ByteCounter_Rx_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void ByteCounter_Rx_SetInterrupt(uint32 interruptMask)
{
    ByteCounter_Rx_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
