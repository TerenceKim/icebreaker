#include <ButtonManager.h>
#include <Application.h>
#include <Button.h>
#include <timers.h>

typedef enum
{
    BM_DURATION_NONE,
    BM_DURATION_PRESS,
    BM_DURATION_PRESS_RELEASE,
    BM_DURATION_LONG_PRESS,
    BM_DURATION_LONG_PRESS_RELEASE,
    BM_DURATION_VLONG_PRESS,
    BM_DURATION_VLONG_PRESS_RELEASE,
    BM_DURATION_VVLONG_PRESS,
    BM_DURATION_VVLONG_PRESS_RELEASE,
    BM_DURATION_VVVLONG_PRESS,
    BM_DURATION_VVVLONG_PRESS_RELEASE
} BM_DURATION_e;

static uint16_t button_hold_timeout_callback(void);
static void buttonManagerHandleButtonEvents(BM_DURATION_e e);

static const uint32_t bm_durations_ms[] =
{
    0,      //    BM_DURATION_NONE,
    1800,   //    BM_DURATION_PRESS,
    1800,   //    BM_DURATION_PRESS_RELEASE,
    3000,   //    BM_DURATION_LONG_PRESS,
    3000,   //    BM_DURATION_LONG_PRESS_RELEASE,
    5000,   //    BM_DURATION_VLONG_PRESS,
    5000,   //    BM_DURATION_VLONG_PRESS_RELEASE,
    15000,  //    BM_DURATION_VVLONG_PRESS,
    15000,  //    BM_DURATION_VVLONG_PRESS_RELEASE,
    0xFFFFFFFF,  //    BM_DURATION_VVVLONG_PRESS,
    0xFFFFFFFF,  //    BM_DURATION_VVVLONG_PRESS_RELEASE
};

volatile uint32_t buttonEvents;
static BM_DURATION_e buttonDuration;
static tmrFuncType buttonHoldTimer = 
{
    .mS = 0, // Set dynamically
    .func = button_hold_timeout_callback
};

static uint16_t button_hold_timeout_callback(void)
{
    buttonDuration += 2; // Skip to next "press", no "release" as of yet

    buttonSetEvents(BUTTON_EVENTS_HOLD_TIMEOUT);
    
    return bm_durations_ms[buttonDuration];   
}

static void buttonManagerStartHoldTimer(void)
{
    // Started by a press, process it
    buttonDuration = BM_DURATION_PRESS;
    buttonManagerHandleButtonEvents(buttonDuration);
    buttonHoldTimer.mS = bm_durations_ms[buttonDuration];
    tmrFuncAdd(&buttonHoldTimer);
}

static void buttonManagerHandleButtonEvents(BM_DURATION_e e)
{
    switch (e)
    {
        case BM_DURATION_PRESS:
        case BM_DURATION_PRESS_RELEASE:
        case BM_DURATION_NONE:
        default:
            break;
    }
}

void buttonManagerInit(void)
{
    buttonEvents = 0;
    buttonDuration = BM_DURATION_NONE;
    buttonInit();
}

void buttonManagerService(void)
{
    if (buttonCheckEvents(BUTTON_EVENTS_LL_PRESS))
    {
        PRINTF("BUTTON_EVENTS_LL_PRESS\n");
        buttonManagerStartHoldTimer();
        buttonClearEvents(BUTTON_EVENTS_LL_PRESS);
    }
    
    if (buttonCheckEvents(BUTTON_EVENTS_LL_RELEASE))
    {
        PRINTF("BUTTON_EVENTS_LL_RELEASE\n");
        // Kill timer
        tmrFuncDelete(&buttonHoldTimer);
        // +1 for "Release" version of "press"
        buttonDuration++;
        // Convert from BM_DURATION_e to BUTTON_EVENTS simply by bit-shifting
        buttonManagerHandleButtonEvents(buttonDuration);
        // Reset button duration
        buttonDuration = BM_DURATION_NONE;
        buttonClearEvents(BUTTON_EVENTS_LL_RELEASE);
    }
    
    if (buttonCheckEvents(BUTTON_EVENTS_HOLD_TIMEOUT))
    {
        PRINTF("BUTTON_EVENTS_HOLD_TIMEOUT: %d\n", buttonDuration);
        buttonManagerHandleButtonEvents(buttonDuration);
        buttonClearEvents(BUTTON_EVENTS_HOLD_TIMEOUT);
    }
}