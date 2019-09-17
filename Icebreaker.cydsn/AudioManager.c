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
#include <AudioManager.h>
#include <Codec.h>
#include <CodecI2CM.h>
#include <Application.h>
#include <I2S.h>
#include <AudioClkSel.h>
#include <TxDMA.h>
#include <isr_TxDMADone.h>
#include <math.h>
#include <timers.h>


volatile uint32_t audioEvents;

#define AUDIO_MANAGER_AFTER_TONE_DELAY_MS       (1000)

#define PI 3.14159

#define SAMPLERATE_HZ 48000  // The sample rate of the audio.  Higher sample rates have better fidelity,
                             // but these tones are so simple it won't make a difference.  44.1khz is
                             // standard CD quality sound.

#define AMPLITUDE     ((1<<23)-1)   // Set the amplitude of generated waveforms.  This controls how loud
                             // the signals are, and can be any value from 0 to 2**31 - 1.  Start with
                             // a low value to prevent damaging speakers!

#define WAV_SIZE      256    // The size of each generated waveform.  The larger the size the higher
                             // quality the signal.  A size of 256 is more than enough for these simple
                             // waveforms.

#define BITS_PER_SAMPLE     24
#define BYTES_PER_SAMPLE    (BITS_PER_SAMPLE / sizeof(uint8_t) * 2) // For stereo


// Define the frequency of music notes (from http://www.phy.mtu.edu/~suits/notefreqs.html):
#define C4_HZ      261.63
#define D4_HZ      293.66
#define E4_HZ      329.63
#define F4_HZ      349.23
#define G4_HZ      392.00
#define A4_HZ      440.00
#define B4_HZ      493.88
#define C5_HZ      523.25

#define OUT_BUFSIZE (WAV_SIZE * BYTES_PER_SAMPLE)

typedef struct
{
  music_note_e note;
  uint32_t     duration_ms;
} music_word_s;

// Define a C-major scale to play all the notes up and down.
float scale[MUSIC_NOTE_MAX] = { C4_HZ, D4_HZ, E4_HZ, F4_HZ, G4_HZ, A4_HZ, B4_HZ, C5_HZ };

// Store basic waveforms in memory.
int32_t sine[WAV_SIZE]     = {0};
uint8 outBuffer[OUT_BUFSIZE];

static const music_word_s cues[AUDIO_CUE_MAX][5] = 
{
  // [AUDIO_CUE_power_on]
  {
    { MUSIC_NOTE_C4, 150 },
    { MUSIC_NOTE_E4, 150 },
    { MUSIC_NOTE_G4, 150 },
    { MUSIC_NOTE_C5, 150 },
    { MUSIC_NOTE_none, 0 }, // END
  },
  // [AUDIO_CUE_start_scan]
  {
    { MUSIC_NOTE_G4, 200 },
    { MUSIC_NOTE_C5, 200 },
    { MUSIC_NOTE_none, 0 }, // END
  },
  // [AUDIO_CUE_connected]
  {
    { MUSIC_NOTE_C5, 50 },
    { MUSIC_NOTE_none, 50 }, 
    { MUSIC_NOTE_C5, 50 },
    { MUSIC_NOTE_none, 0 }, // END
  },
  // [AUDIO_CUE_disconnected]
  {
    { MUSIC_NOTE_A4, 200 },
    { MUSIC_NOTE_D4, 200 },
    { MUSIC_NOTE_none, 0 }, // END
  },
  // [AUDIO_CUE_power_off]
  {
    { MUSIC_NOTE_C5, 150 },
    { MUSIC_NOTE_G4, 150 },
    { MUSIC_NOTE_E4, 150 },
    { MUSIC_NOTE_C4, 150 },
    { MUSIC_NOTE_none, 0 }, // END
  },
};

typedef struct
{
  uint32_t tmrEvent;
  tmrFuncType tmr;
  audio_routing_e routing;
  music_word_s *pCurrentPhrase;
  music_word_s *pQueuedPhrase;
  
  audio_routing_e routing_old;
} audio_manager_cb_s;

static audio_manager_cb_s audioManagerCb;

static uint16_t rfControllerSendEvent(void)
{
  audioSetEvents(audioManagerCb.tmrEvent);

  return 0;
}

static void audioClearLaterEvents(void)
{
  tmrFuncDelete(&audioManagerCb.tmr);
}

static void audioSetEventsLater(uint32_t e, uint16_t mS)
{
  audioManagerCb.tmr.mS = mS;
  audioManagerCb.tmr.func = rfControllerSendEvent;
  audioManagerCb.tmrEvent = e;
  tmrFuncAdd(&audioManagerCb.tmr);
}

static void generateSineByFreq(float frequency, int32_t amplitude, int32_t *buffer, uint16_t *pLength)
{
  float newLength = (float)SAMPLERATE_HZ / frequency;
  
  *pLength = (uint16_t)newLength;

  // Generate a sine wave signal with the provided amplitude and store it in
  // the provided buffer of size length.
  for (int i=0; i<*pLength; ++i) {
    buffer[i] = (int32_t)((float)(amplitude)*sin(2.0*PI*(1.0/(*pLength))*i));
  }
}

void AudioManagerInit(void)
{
  audioEvents = 0;
      
  /* Set the default Audio clock rate to 48 kHz */
  AudioClkSel_Write(RATE_48KHZ);
  
  /* Enable DMA */
  CyDmaEnable();
  
  TxDMA_Init();
  
  /* Start I2C Master */
  CodecI2CM_Start();

	if(Codec_Init() == 0)
	{
		D_PRINTF(INFO, "Codec comm works!... \r\n");
	}
	else
	{
		D_PRINTF(ERROR, "Codec comm DOESN'T work!... \r\n");
	}
  
  /* Disable power to speaker output */
  Codec_PowerOffControl(CODEC_POWER_CTRL_OUTPD);
}

static void audioManagerNotePlaybackStart(music_note_e note)
{
  uint32_t i;
  int32_t sample;
  uint16_t length;
  
  if (note == MUSIC_NOTE_none)
  {
    return;
  }

  generateSineByFreq(scale[note], AMPLITUDE, sine, &length);
  
  for (i = 0; i < length; i++)
  {
    sample = sine[i];

    outBuffer[i*6+5] = (sample >> 16) & 0xFF;
    outBuffer[i*6+4] = (sample >> 8) & 0xFF;
    outBuffer[i*6+3] = sample & 0xFF;
    
    outBuffer[i*6+2] = (sample >> 16) & 0xFF;
    outBuffer[i*6+1] = (sample >> 8) & 0xFF;
    outBuffer[i*6+0] = sample & 0xFF;
  }
  
  TxDMA_SetNumDataElements(0, length*6);
  TxDMA_SetSrcAddress(0, (void *) outBuffer);
	TxDMA_SetDstAddress(0, (void *) I2S_TX_FIFO_0_PTR);

  /* Validate descriptor */
  TxDMA_ValidateDescriptor(0);
  
  I2S_ClearTxFIFO(); /* Clear the I2S internal FIFO */
  
  Codec_SetMute(false);
  
  I2S_EnableTx(); /* Unmute the TX output */
  
  TxDMA_ChEnable();
}

static void audioManagerNotePlaybackStop(void)
{
  Codec_SetMute(true);
  I2S_DisableTx(); /* Mute the TX output */
  
  TxDMA_ChDisable();
}

static void audioManagerPlayPhrase(music_word_s *pPhrase)
{
  audio_routing_e routing_old = AudioManagerGetRouting();
  
  if (audioManagerCb.pCurrentPhrase && audioManagerCb.pCurrentPhrase->duration_ms != 0)
  {
    audioManagerCb.pQueuedPhrase = pPhrase;
    return;
  }
  
  AudioManagerSetRouting(AUDIO_ROUTING_mcu);
  
  audioManagerCb.pCurrentPhrase = pPhrase;

  audioManagerNotePlaybackStart(audioManagerCb.pCurrentPhrase->note);
  
  audioSetEventsLater(AUDIO_EVENTS_PHRASE_PLAYBACK_CONTINUE, audioManagerCb.pCurrentPhrase->duration_ms);
  audioManagerCb.pCurrentPhrase++;
}

void AudioManagerTonePlay(music_note_e note, uint32_t durationMs)
{
  audioManagerCb.routing_old = AudioManagerGetRouting();
  AudioManagerSetRouting(AUDIO_ROUTING_mcu);
  
  audioManagerNotePlaybackStart(note);
  
  CyDelay(durationMs);
  
  audioManagerNotePlaybackStop();
  
  AudioManagerSetRouting(audioManagerCb.routing_old);
}

void AudioManagerCuePlay(audio_cue_e cue)
{
  audioManagerPlayPhrase((music_word_s *)cues[cue]);
}

void AudioManagerService(void)
{
  if (audioCheckEvents(AUDIO_EVENTS_PHRASE_PLAYBACK_CONTINUE))
  {
    audioClearEvents(AUDIO_EVENTS_PHRASE_PLAYBACK_CONTINUE);
    
    if (audioManagerCb.pCurrentPhrase && audioManagerCb.pCurrentPhrase->duration_ms)
    {
      audioManagerNotePlaybackStop();
      
      audioManagerNotePlaybackStart(audioManagerCb.pCurrentPhrase->note);
      
      audioSetEventsLater(AUDIO_EVENTS_PHRASE_PLAYBACK_CONTINUE, audioManagerCb.pCurrentPhrase->duration_ms);
      audioManagerCb.pCurrentPhrase++;
    }
    else
    {
      AudioManagerSetRouting(audioManagerCb.routing_old);
      audioManagerCb.pCurrentPhrase = NULL;
      
      // Give some silence before next cue
      audioSetEventsLater(AUDIO_EVENTS_PHRASE_PLAYBACK_DONE, AUDIO_MANAGER_AFTER_TONE_DELAY_MS);
    }
  }
  
  if (audioCheckEvents(AUDIO_EVENTS_PHRASE_PLAYBACK_DONE))
  {
    audioClearEvents(AUDIO_EVENTS_PHRASE_PLAYBACK_DONE);
    
    if (audioManagerCb.pQueuedPhrase)
    {
      audioManagerPlayPhrase(audioManagerCb.pQueuedPhrase);
      audioManagerCb.pQueuedPhrase = NULL;
    }
  } 
}

void AudioManagerSetRouting(audio_routing_e routing)
{
  if (audioManagerCb.routing == routing)
  {
    return;
  }
  
  switch (routing)
  {
    case AUDIO_ROUTING_none:
    {
      /* Disable power to speaker output to save power */
      Codec_PowerOffControl(CODEC_POWER_CTRL_OUTPD);
      
      I2S_Stop();
    } break;
    
    case AUDIO_ROUTING_mcu:
    {      
      Codec_Deactivate();
      Codec_SendData(CODEC_REG_DIGITAL_IF, CODEC_DIGITAL_IF_IWL_24_BIT | CODEC_DIGITAL_IF_FORMAT_I2S);
      Codec_SetSamplingRate(CODEC_SRATE_NORMAL_48KHZ_256FS);
      Codec_Activate();

      I2S_Start();
      
      /* Enable power to speaker output */
      Codec_PowerOnControl(CODEC_POWER_CTRL_OUTPD);
      
      CyDelay(10);
    } break;
    
    case AUDIO_ROUTING_rfc:
    {   
      I2S_Stop();
      
      Codec_Deactivate();
      Codec_SendData(CODEC_REG_DIGITAL_IF, CODEC_DIGITAL_IF_IWL_16_BIT | CODEC_DIGITAL_IF_FORMAT_I2S);
      Codec_SetSamplingRate(CODEC_SRATE_NORMAL_44KHZ_256FS);
      Codec_Activate();
      
      /* Enable power to speaker output */
      Codec_PowerOnControl(CODEC_POWER_CTRL_OUTPD);
      
      CyDelay(10);
    } break;
    
    default:
      break;
  }
  audioManagerCb.routing = routing;
}

audio_routing_e AudioManagerGetRouting(void)
{
  return audioManagerCb.routing; 
}


/* [] END OF FILE */
