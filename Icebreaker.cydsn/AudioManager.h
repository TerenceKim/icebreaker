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
  
typedef enum
{
  MUSIC_NOTE_C4,
  MUSIC_NOTE_D4,
  MUSIC_NOTE_E4,
  MUSIC_NOTE_F4,
  MUSIC_NOTE_G4,
  MUSIC_NOTE_A4,
  MUSIC_NOTE_B4,
  MUSIC_NOTE_C5,
  MUSIC_NOTE_none,
  MUSIC_NOTE_MAX
} music_note_e;

typedef enum
{
  AUDIO_CUE_power_on,
  AUDIO_CUE_start_scan,
  AUDIO_CUE_connected,
  AUDIO_CUE_disconnected,
  AUDIO_CUE_power_off,
  AUDIO_CUE_MAX
} audio_cue_e;

typedef enum
{
  AUDIO_ROUTING_none,
  AUDIO_ROUTING_mcu,
  AUDIO_ROUTING_rfc
} audio_routing_e;

typedef struct
{
  music_note_e note;
  uint32_t     duration_ms;
} music_word_s;

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
void AudioManagerSetRouting(audio_routing_e routing);
audio_routing_e AudioManagerGetRouting(void);

void AudioManagerTonePlay(music_note_e note, uint32_t durationMs);
void AudioManagerCuePlay(audio_cue_e cue);

#endif /* AUDIO_MANAGER_H */
/* [] END OF FILE */
