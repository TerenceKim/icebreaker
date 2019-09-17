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
#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <stdint.h>
  
extern volatile uint32_t sysEvents;
  
#define sysSetEvents(e)     { sysEvents |= (e); }
#define sysCheckEvents(e)   ((sysEvents & (e)) != 0)
#define sysClearEvents(e)   { sysEvents &= ~(e); }
  
#define SYS_EVENTS_UE_POWER_ON                  (1 <<  0)
#define SYS_EVENTS_UE_POWER_OFF                 (1 <<  1)
#define SYS_EVENTS_UE_CHARGER_CONNECTED         (1 <<  2)
#define SYS_EVENTS_UE_CHARGER_DISCONNECTED      (1 <<  3)
#define SYS_EVENTS_UE_ENTER_PAIRING             (1 <<  4)
#define SYS_EVENTS_UE_SWITCH_ROLE               (1 <<  5)
#define SYS_EVENTS_NWK_JOINED                   (1 <<  6)
#define SYS_EVENTS_NWK_LOST                     (1 <<  7)
  
void SystemManagerService(void);
  
#endif /* SYSTEM_MANAGER_H */

/* [] END OF FILE */
