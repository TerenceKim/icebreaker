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
#include <Features.h>
#include <SystemManager.h>
#include <AudioManager.h>
#include <RfController.h>
#include <LedManager.h>

volatile uint32_t sysEvents;



void SystemManagerService(void)
{
  if (sysCheckEvents(SYS_EVENTS_UE_POWER_ON))
  {
    sysClearEvents(SYS_EVENTS_UE_POWER_ON);

    if (RfControllerGetState() == NWK_STATE_idle)
    {
      // TODO: Check for critical battery and show animation here
      LedManagerSeqPlay(LED_SEQ_event_power_on);

      AudioManagerCuePlay(AUDIO_CUE_power_on);
      
      rfSetEvents(RF_EVENTS_POWER_ON);
    }
  }
  
  if (sysCheckEvents(SYS_EVENTS_UE_POWER_OFF))
  {
    sysClearEvents(SYS_EVENTS_UE_POWER_OFF);
    
    if (RfControllerGetState() != NWK_STATE_idle)
    {
      LedManagerSeqPlay(LED_SEQ_event_power_off);

      AudioManagerCuePlay(AUDIO_CUE_power_off);
      
      rfSetEvents(RF_EVENTS_POWER_OFF);
    }
  }
  
  if (sysCheckEvents(SYS_EVENTS_UE_ENTER_PAIRING))
  {
    sysClearEvents(SYS_EVENTS_UE_ENTER_PAIRING);
    
    if (IS_SLAVE() && RfControllerGetState() != NWK_STATE_idle)
    {
      LedManagerSeqPlay(LED_SEQ_state_scanning);

      AudioManagerCuePlay(AUDIO_CUE_start_scan);

      rfSetEvents(RF_EVENTS_SCAN_START);
    }
  }
  
  if (sysCheckEvents(SYS_EVENTS_UE_AUTO_CONNECT))
  {
    sysClearEvents(SYS_EVENTS_UE_AUTO_CONNECT);
    
    if ((RfControllerGetState() != NWK_STATE_connected) &&
        (IS_SLAVE()))
    {
      LedManagerSeqPlay(LED_SEQ_state_auto_conn);
      
      rfSetEvents(RF_EVENTS_AUTO_CONNECT);
    }
  }

  if (sysCheckEvents(SYS_EVENTS_UE_SHOW_LEVEL))
  {
    sysClearEvents(SYS_EVENTS_UE_SHOW_LEVEL);
    
    LedManagerSeqPlay(LED_SEQ_event_show_level);
  }
  
  if (sysCheckEvents(SYS_EVENTS_NWK_JOINED))
  {
    sysClearEvents(SYS_EVENTS_NWK_JOINED);
    
    if (RfControllerGetState() == NWK_STATE_connected)
    {
      LedManagerSeqPlay(LED_SEQ_event_nwk_joined);

      // Only for slaves
      AudioManagerCuePlay(AUDIO_CUE_connected);
    }
  }
  
  if (sysCheckEvents(SYS_EVENTS_NWK_LOST))
  {
    sysClearEvents(SYS_EVENTS_NWK_LOST);
    
    if ((RfControllerGetState() == NWK_STATE_connected) &&
        (IS_SLAVE()))
    {
      LedManagerSeqPlay(LED_SEQ_state_auto_conn);

      AudioManagerCuePlay(AUDIO_CUE_disconnected);
      
      rfSetEvents(RF_EVENTS_AUTO_CONNECT);
    }
  }

  if (sysCheckEvents(SYS_EVENTS_UE_DISCOVERABLE))
  {
    sysClearEvents(SYS_EVENTS_UE_DISCOVERABLE);

    LedManagerSeqPlay(LED_SEQ_state_connectable);

    rfSetEvents(RF_EVENTS_CONTROL_ENABLE);
  }

#ifdef FEATURE_USER_ROLE_SWITCH
  if (sysCheckEvents(SYS_EVENTS_UE_SWITCH_ROLE))
  {
    sysClearEvents(SYS_EVENTS_UE_SWITCH_ROLE);
    
    RfControllerRunMode(RUN_MODE_app, (RfControllerGetRole() == PROTOCOL_ROLE_slave) ? PROTOCOL_ROLE_master : PROTOCOL_ROLE_slave);

    // TODO: LED feedback
  }
#endif /* FEATURE_USER_ROLE_SWITCH */
}

/* [] END OF FILE */
