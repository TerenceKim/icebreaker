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
#ifndef TK_TIMERS_H
#define TK_TIMERS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

extern volatile uint32_t tmrCounter_ms;

typedef struct _tmrFuncType
{
  struct _tmrFuncType *next;
  uint32_t mS;
  uint16_t (*func)(void);
} tmrFuncType;

#define tmrGetCounter_ms()          (tmrCounter_ms)
#define tmrGetElapsedMs(start)      (tmrCounter_ms - start)
#define tmrGetTimeToExpiry(pTmr)    (pTmr->mS)
    
void tmrInit(void);
void tmrService(void);
bool tmrIsRunning(void);
void tmrFuncAdd(tmrFuncType *pTmr);
void tmrFuncDelete(tmrFuncType *pTmr);


#endif /* TK_TIMERS_H */

/* [] END OF FILE */
