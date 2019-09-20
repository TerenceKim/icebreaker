#include <Button.h>
#include <timers.h>
#include <BTN.h>
#include <isr_Button.h>
#include <ButtonManager.h>
#include <Application.h>

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
  BUTTON_DEBOUNCE_EVT_SAMPLE
} BUTTON_DEBOUNCE_EVT_e;

static BUTTON_DEBOUNCE_EVT_e lastButtonEvent;
static BUTTON_DEBOUNCE_EVT_e lastOfficialButtonEvent;
static uint8_t buttonVotesOfConfidence;
static BUTTON_DEBOUNCE_STATE_e buttonDebounceState;

static uint16_t button_debounce_timer_callback(void);
static BUTTON_DEBOUNCE_STATE_e buttonDebounceGetState(void);
static void buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_e state);

static tmrFuncType buttonDebounceTimer = 
{
  .mS = BUTTON_DEBOUNCE_INTERVAL_MS,
  .func = button_debounce_timer_callback
};

static uint16_t button_debounce_timer_callback(void)
{
  uint16_t ret = 0;
  BUTTON_DEBOUNCE_EVT_e evt = (BTN_Read()) ? BUTTON_DEBOUNCE_EVT_PRESSED : BUTTON_DEBOUNCE_EVT_RELEASED;
  
  switch (buttonDebounceGetState())
  {
    case BUTTON_DEBOUNCE_STATE_DEBOUNCING:
    {
      if (lastButtonEvent == evt)
      {
        if (++buttonVotesOfConfidence >= NUM_OF_DEBOUNCE_CHECKS)
        {
          buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_DONE);
        }
        else
        {
          // Kick timer for next sample
          ret = BUTTON_DEBOUNCE_INTERVAL_MS;
        }
      }
      else
      {
        // Lost all confidence - start over w/ sampling with new POR
        buttonVotesOfConfidence = 0;
        lastButtonEvent = evt;
        ret = BUTTON_DEBOUNCE_INTERVAL_MS;
      }
    } break;
    
    case BUTTON_DEBOUNCE_STATE_IDLE:
    case BUTTON_DEBOUNCE_STATE_DONE:
    default:
    {
      // Kill straggler timer
      ret = 0;
    } break;
  }
  
  return ret;
}

static BUTTON_DEBOUNCE_STATE_e buttonDebounceGetState(void)
{
    return buttonDebounceState;
}

static void buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_e state)
{
  switch (state)
  {
    case BUTTON_DEBOUNCE_STATE_DONE:
    {
      if (lastButtonEvent != lastOfficialButtonEvent)
      {
        if (lastButtonEvent == BUTTON_DEBOUNCE_EVT_PRESSED)
        {
          buttonSetEvents(BUTTON_EVENTS_LL_PRESS);
        }
        else
        {
          buttonSetEvents(BUTTON_EVENTS_LL_RELEASE);
        }
        lastOfficialButtonEvent = lastButtonEvent;
      }
      buttonDebounceState = BUTTON_DEBOUNCE_STATE_IDLE;
    } break;

    case BUTTON_DEBOUNCE_STATE_IDLE:
    {
      buttonVotesOfConfidence = 0;
      buttonDebounceState = BUTTON_DEBOUNCE_STATE_IDLE;
    } break;

    case BUTTON_DEBOUNCE_STATE_DEBOUNCING:
    {
      buttonDebounceTimer.mS = BUTTON_DEBOUNCE_INTERVAL_MS;
      tmrFuncAdd(&buttonDebounceTimer);
      buttonDebounceState = BUTTON_DEBOUNCE_STATE_DEBOUNCING;
    } break;

    default:
      break;
  }
}

void buttonInit(void)
{
  buttonDebounceState = BUTTON_DEBOUNCE_STATE_IDLE;
  lastButtonEvent = BUTTON_DEBOUNCE_EVT_RELEASED;
  lastOfficialButtonEvent = BUTTON_DEBOUNCE_EVT_RELEASED;
  buttonVotesOfConfidence = 0;
  
  isr_Button_Start();
  
  // Run first time check in case User is still pressing button from boot-up
  buttonEdgeDetected();
}

void buttonEdgeDetected(void)
{
  if (buttonDebounceGetState() == BUTTON_DEBOUNCE_STATE_IDLE)
  {
    lastButtonEvent = (BTN_Read()) ? BUTTON_DEBOUNCE_EVT_PRESSED : BUTTON_DEBOUNCE_EVT_RELEASED;
    buttonDebounceSetState(BUTTON_DEBOUNCE_STATE_DEBOUNCING);
  }
}
