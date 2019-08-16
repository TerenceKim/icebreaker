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
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <stdint.h>

/* Clock Rates */
#define RATE_48KHZ                                   0
#define RATE_44KHZ                                   1

#define AUDIO_EVENTS_I2S_TX_DMA_DONE                 (1 <<  0)
  
extern volatile uint32_t audioEvents;
  
#define audioSetEvents(e)     { audioEvents |= (e); }
#define audioCheckEvents(e)   ((audioEvents & (e)) != 0)
#define audioClearEvents(e)   { audioEvents &= ~(e); } 

void AudioManagerInit(void);
void AudioManagerService(void);

void AudioManagerToneStart(uint32_t word);
void AudioManagerToneStop(void);

#endif /* AUDIO_MANAGER_H */
/* [] END OF FILE */
