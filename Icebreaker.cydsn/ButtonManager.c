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

// The values below stack on top of each other
static const uint32_t bm_durations_ms[] =
{
    1800,   //    BM_DURATION_PRESS,
    1200,   //    BM_DURATION_LONG_PRESS,
    2000,   //    BM_DURATION_VLONG_PRESS,
    10000,  //    BM_DURATION_VVLONG_PRESS,
    0xFFFFFFFF,  //    BM_DURATION_VVVLONG_PRESS,
};

#define BM_GET_NEXT_LEVEL_TIMEOUT(b)        (bm_durations_ms[(b-1)/2])

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
    
    return BM_GET_NEXT_LEVEL_TIMEOUT(buttonDuration);   
}

static void buttonManagerStartHoldTimer(void)
{
    // Started by a press, process it
    buttonDuration = BM_DURATION_PRESS;
    buttonManagerHandleButtonEvents(buttonDuration);
    buttonHoldTimer.mS = BM_GET_NEXT_LEVEL_TIMEOUT(buttonDuration);
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
        PRINTF("[%08lu] BUTTON_EVENTS_LL_PRESS\n", tmrGetCounter_ms());
        buttonManagerStartHoldTimer();
        buttonClearEvents(BUTTON_EVENTS_LL_PRESS);
    }
    
    if (buttonCheckEvents(BUTTON_EVENTS_LL_RELEASE))
    {
        // Kill timer
        tmrFuncDelete(&buttonHoldTimer);
        // +1 for "Release" version of "press"
        buttonDuration++;
        PRINTF("[%08lu] BUTTON_EVENTS_LL_RELEASE: %d\n", tmrGetCounter_ms(), buttonDuration);
        // Convert from BM_DURATION_e to BUTTON_EVENTS simply by bit-shifting
        buttonManagerHandleButtonEvents(buttonDuration);
        // Reset button duration
        buttonDuration = BM_DURATION_NONE;
        buttonClearEvents(BUTTON_EVENTS_LL_RELEASE);
    }
    
    if (buttonCheckEvents(BUTTON_EVENTS_HOLD_TIMEOUT))
    {
        PRINTF("[%08lu] BUTTON_EVENTS_HOLD_TIMEOUT: %d\n", tmrGetCounter_ms(), buttonDuration);
        buttonManagerHandleButtonEvents(buttonDuration);
        buttonClearEvents(BUTTON_EVENTS_HOLD_TIMEOUT);
    }
}