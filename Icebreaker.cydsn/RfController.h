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
  
#define RF_SCAN_TIME_SECONDS          (10)

#define RF_EVENTS_POWER_ON            (1 <<  0)
#define RF_EVENTS_POWER_OFF           (1 <<  1)
#define RF_EVENTS_EHIF_IRQ            (1 <<  2)
#define RF_EVENTS_SCAN_START          (1 <<  3)
#define RF_EVENTS_SCAN_STOP           (1 <<  4)

extern volatile uint32_t rfEvents;
  
#define rfSetEvents(e)     { rfEvents |= (e); }
#define rfCheckEvents(e)   ((rfEvents & (e)) != 0)
#define rfClearEvents(e)   { rfEvents &= ~(e); } 

void RfControllerInit(void);
void RfControllerService(void);
bool RfControllerPrintInfo(void);
void RfControllerStartScan(void);
bool RfControllerPrintScanResults(void);
void RfControllerPrintStats(void);

#endif /* RF_CONTROLLER_H */