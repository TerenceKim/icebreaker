/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <LedManager.h>
#include <LED_RED_PWM_ISR.h>
#include <LED_BLUE_PWM_ISR.h>
#include <LED_RED_PWM.h>
#include <LED_BLUE_PWM.h>
#include <stdbool.h>

typedef struct
{
    void (* startIsr)(void);
    void (* start)(void);
    void (* stop)(void);
    void (* writeCounter)(uint32 count);
    uint32 (* readCounter)(void);
    void (* writePeriod)(uint32 period);
    uint32 (* readPeriod)(void);
    void (* writeCompare)(uint32 compare);
    uint32 (* readCompare)(void);
} pwm_callbacks_s;

static pwm_callbacks_s pwm[LM_LED_MAX] = 
{
    // LM_LED_RED
    {
        .startIsr = LED_RED_PWM_ISR_Start,
        .start = LED_RED_PWM_Start,
        .stop = LED_RED_PWM_Stop,
        .writeCounter = LED_RED_PWM_WriteCounter,
        .readCounter = LED_RED_PWM_ReadCounter,
        .writePeriod = LED_RED_PWM_WritePeriod,
        .readPeriod = LED_RED_PWM_ReadPeriod,
        .writeCompare = LED_RED_PWM_WriteCompare,
        .readCompare = LED_RED_PWM_ReadCompare
    },
    // LM_LED_GREEN
    {
//        .startIsr = LED_GREEN_PWM_ISR_Start,
//        .start = LED_GREEN_PWM_Start,
//        .stop = LED_GREEN_PWM_Stop,
//        .writeCounter = LED_GREEN_PWM_WriteCounter,
//        .readCounter = LED_GREEN_PWM_ReadCounter,
//        .writePeriod = LED_GREEN_PWM_WritePeriod,
//        .readPeriod = LED_GREEN_PWM_ReadPeriod,
//        .writeCompare = LED_GREEN_PWM_WriteCompare,
//        .readCompare = LED_GREEN_PWM_ReadCompare
    },
    // LM_LED_BLUE
    {
        .startIsr = LED_BLUE_PWM_ISR_Start,
        .start = LED_BLUE_PWM_Start,
        .stop = LED_BLUE_PWM_Stop,
        .writeCounter = LED_BLUE_PWM_WriteCounter,
        .readCounter = LED_BLUE_PWM_ReadCounter,
        .writePeriod = LED_BLUE_PWM_WritePeriod,
        .readPeriod = LED_BLUE_PWM_ReadPeriod,
        .writeCompare = LED_BLUE_PWM_WriteCompare,
        .readCompare = LED_BLUE_PWM_ReadCompare
    },
};

static bool override[LM_LED_MAX];
static int32 change[LM_LED_MAX];

void LedManagerStartOverride(LM_LED_e ledIdx, uint32_t pwmPeriod, uint32_t pwmCompare, uint32_t pwmCounter)
{
    override[ledIdx] = true;

    pwm[ledIdx].startIsr();
    pwm[ledIdx].start();
    pwm[ledIdx].writePeriod(pwmPeriod);
    pwm[ledIdx].writeCounter(pwmCounter);
    pwm[ledIdx].writeCompare(pwmCompare);
    
    change[ledIdx] = pwmCompare;
}

void LedManagerStopOverride(LM_LED_e ledIdx)
{
    pwm[ledIdx].stop();
    override[ledIdx] = false;
}

void LedManagerInterruptHandler(LM_LED_e ledIdx)
{
    if (override[ledIdx])
    {
        switch (ledIdx)
        {
            case LM_LED_RED:
                LED_RED_PWM_ClearInterrupt(LED_RED_PWM_INTR_MASK_TC);
                break;
            
            case LM_LED_BLUE:
                LED_BLUE_PWM_ClearInterrupt(LED_BLUE_PWM_INTR_MASK_TC);
                break;

            default:
                break;
        }
        
        if ((change[ledIdx] > 0 && (pwm[ledIdx].readCompare() + change[ledIdx]) <= pwm[ledIdx].readPeriod()) ||
            (change[ledIdx] < 0 && ((int32)pwm[ledIdx].readCompare() + change[ledIdx]) >= 1))
        {
            pwm[ledIdx].writeCompare(pwm[ledIdx].readCompare() + change[ledIdx]);
        }
        else
        {
            // Change direction (next go around)
            change[ledIdx] *= -1;
        }
    }
}

/* [] END OF FILE */
