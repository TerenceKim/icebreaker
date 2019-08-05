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
#include <Application.h>
#include <timers.h>
#include <stdio.h>
#include <stdbool.h>

volatile uint32_t tmrCounter_ms;

static tmrFuncType headTmr;
    
void tmrInit(void)
{
    headTmr.next = NULL;
    headTmr.mS = 0;
    headTmr.func = NULL;
}

void tmrService(void)
{
    tmrFuncType *pTmr = &headTmr;
    
    tmrCounter_ms++;
    
    while (pTmr->next)
    {
        if (pTmr->next->mS == 0)
        {
            // Timer has expired - execute function
            pTmr->next->mS = pTmr->next->func();
            if (pTmr->next->mS == 0)
            {
                // Timer was not kicked - remove from list
                pTmr->next = pTmr->next->next;
            }
        }
        else
        {
            pTmr->next->mS--;
        }
        
        if (pTmr->next)
        {
            // Advance pointer
            pTmr = pTmr->next;
        }
        else
        {
            // Reached the end of the list
            break;
        }
    }
}

bool tmrIsRunning(void)
{
    bool running = false;
    tmrFuncType *pTmr = &headTmr;
    
    while (pTmr->next)
    {
        if (pTmr->next->mS)
        {
            running = true;
            break;
        }
        pTmr = pTmr->next;
    }
    return running;
}

void tmrFuncAdd(tmrFuncType *pTmr)
{
    tmrFuncType *p = &headTmr;
    
    if (!pTmr->mS || !pTmr->func)
    {
        // Invalid arguments
        return;
    }
    
    // Go to end of list
    while (p->next)
    {
        if (p->next != pTmr)
        {
            p = p->next;
        }
        else
        {
            // Timer already running - run with new times
            p->next->mS = pTmr->mS;
            return;
        }
    }
    
    // Add to end of list
    pTmr->next = NULL;
    p->next = pTmr;
}

void tmrFuncDelete(tmrFuncType *pTmr)
{
    tmrFuncType *p = &headTmr;
    
    while (p->next)
    {
        if (p->next == pTmr)
        {
            p->next = p->next->next;
            break;
        }
        
        p = p->next;
    }
}

