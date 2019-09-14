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
#ifndef RF_CONTROLLER_H
#define RF_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

#define RF_EVENTS_POWER_ON            (1 <<  0)
#define RF_EVENTS_POWER_OFF           (1 <<  1)
#define RF_EVENTS_EHIF_IRQ            (1 <<  2)
#define RF_EVENTS_SCAN_START          (1 <<  3)
#define RF_EVENTS_SCAN_CHECK          (1 <<  4)
#define RF_EVENTS_SCAN_STOP			      (1 <<  5)
#define RF_EVENTS_JOIN_CHECK		      (1 <<  6)
#define RF_EVENTS_AUTO_CONN_CHECK	    (1 <<  7)

extern volatile uint32_t rfEvents;
  
#define rfSetEvents(e)     { rfEvents |= (e); }
#define rfCheckEvents(e)   ((rfEvents & (e)) != 0)
#define rfClearEvents(e)   { rfEvents &= ~(e); } 

void RfControllerInit(void);
void RfControllerService(void);
bool RfControllerPrintInfo(void);
void RfControllerNetworkScan(void);
bool RfControllerPrintScanResults(void);
void RfControllerPrintStats(void);
void RfControllerAutoConnect(void);
void RfControllerNetworkJoin(uint32_t deviceId);

#endif /* RF_CONTROLLER_H */