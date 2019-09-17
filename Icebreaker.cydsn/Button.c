#include <Button.h>
#include <timers.h>
#include <BTN.h>
#include <isr_Button.h>
#include <ButtonManager.h>

#define NUM_OF_DEBOUNCE_CHECKS      (5)
#define BUTTON_DEBOUNCE_INTERVAL_MS (20)

typedef enum
{
    BUTTON_DEBOUNCE_STATE_IDLE,
    BUTTON_DEBOUNCE_STATE_DEBOUNCING,
    BUTTON_DEBOUNCE_STATE_DONE
} BUTTON_DEBOUNCE_STATE_e;

typedef enum
{
    BUTTON_DEBOUNCE_EVT_PRESSED,
    BUTTON_DEBOUNCE_EVT_RELEASED,
} BUTTON_DEBOUNCE_EVT_e;

static bool buttonPressed;
static bool buttonOfficiallyPressed;
static uint8_t buttonVotesOfConfidence;
static BUTTON_DEBOUNCE_STATE_e buttonDebounceState;

static uint16_t button_debounce_timer_callback(void);
static void buttonDebounceEventHandler(BUTTON_DEBOUNCE_EVT_e e);
static BUTTON_DEBOUNCE_STATE_e buttonDebounceGetState(void);

static tmrFuncType buttonDebounceTimer = 
{
    .mS = BUTTON_DEBOUNCE_INTERVAL_MS,
    .func = button_debounce_timer_callback
};

static uint16_t button_debounce_timer_callback(void)
{
    bool pressed = BTN_Read();
    buttonDebounceEventHandler((pressed) ? BUTTON_DEBOUNCE_EVT_PRESSED : BUTTON_DEBOUNCE_EVT_RELEASED);
    buttonPressed = pressed;
    
    if (buttonDebounceGetState() == BUTTON_DEBOUNCE_STATE_DEBOUNCING)
    {
        return BUTTON_DEBOUNCE_INTERVAL_MS;
    }
    
    return 0;
}

static BUTTON_DEBOUNCE_STATE_e buttonDebounceGetState(void)
{
    return buttonDebounceState;
}

static void buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_e state)
{
    switch (state)
    {
        case BUTTON_DEBOUNCE_STATE_IDLE:
            buttonVotesOfConfidence = 0;
            buttonDebounceState = BUTTON_DEBOUNCE_STATE_IDLE;
            break;
        case BUTTON_DEBOUNCE_STATE_DEBOUNCING:
            buttonDebounceState = BUTTON_DEBOUNCE_STATE_DEBOUNCING;
            break;
        case BUTTON_DEBOUNCE_STATE_DONE:
            if (buttonPressed != buttonOfficiallyPressed)
            {
                if (buttonPressed)
                {
                    buttonSetEvents(BUTTON_EVENTS_LL_PRESS);
                }
                else
                {
                    buttonSetEvents(BUTTON_EVENTS_LL_RELEASE);
                }
                buttonOfficiallyPressed = buttonPressed;
            }
            buttonDebounceState = BUTTON_DEBOUNCE_STATE_IDLE;
            break;
        default:
            break;
    }
}

static void buttonDebounceEventHandler(BUTTON_DEBOUNCE_EVT_e e)
{
    switch (e)
    {
        case BUTTON_DEBOUNCE_EVT_PRESSED:
        case BUTTON_DEBOUNCE_EVT_RELEASED:
            if (((buttonPressed == true) && (e == BUTTON_DEBOUNCE_EVT_PRESSED)) ||
                ((buttonPressed == false) && (e == BUTTON_DEBOUNCE_EVT_RELEASED)))
            {
                // Affirming more of the same
                buttonVotesOfConfidence++;
                if (buttonVotesOfConfidence >= NUM_OF_DEBOUNCE_CHECKS)
                {
                    buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_DONE);
                }
                else
                {
                    buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_DEBOUNCING);
                }
            }
            else
            {
                // Found a debounce, reset detection logic
                buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_DEBOUNCING);
                buttonVotesOfConfidence = 0;
            }
            break;
        default:
            break;
    }
    
    
}

void buttonInit(void)
{
    buttonDebounceState = BUTTON_DEBOUNCE_STATE_IDLE;
    buttonPressed = false;
    buttonOfficiallyPressed = false;
    buttonVotesOfConfidence = 0;
    
    isr_Button_Start();
    
    // Run first time check in case User is still pressing button from boot-up
    buttonEdgeDetected();
}

void buttonEdgeDetected(void)
{
    bool pressed = BTN_Read();
    buttonDebounceEventHandler((pressed) ? BUTTON_DEBOUNCE_EVT_PRESSED : BUTTON_DEBOUNCE_EVT_RELEASED);

    buttonPressed = pressed;

    if (buttonDebounceGetState() == BUTTON_DEBOUNCE_STATE_DEBOUNCING)
    {
        // Clear any running timer first
        tmrFuncDelete(&buttonDebounceTimer);

        // Start debounce timer
        buttonDebounceTimer.mS = BUTTON_DEBOUNCE_INTERVAL_MS;
        tmrFuncAdd(&buttonDebounceTimer);
    }
}
