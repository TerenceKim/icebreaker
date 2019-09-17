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

#define PI 3.14159

volatile uint32_t audioEvents;

#define SAMPLERATE_HZ 48000  // The sample rate of the audio.  Higher sample rates have better fidelity,
                             // but these tones are so simple it won't make a difference.  44.1khz is
                             // standard CD quality sound.

#define AMPLITUDE     ((1<<23)-1)   // Set the amplitude of generated waveforms.  This controls how loud
                             // the signals are, and can be any value from 0 to 2**31 - 1.  Start with
                             // a low value to prevent damaging speakers!

#define WAV_SIZE      256    // The size of each generated waveform.  The larger the size the higher
                             // quality the signal.  A size of 256 is more than enough for these simple
                             // waveforms.


// Define the frequency of music notes (from http://www.phy.mtu.edu/~suits/notefreqs.html):
#define C4_HZ      261.63
#define D4_HZ      293.66
#define E4_HZ      329.63
#define F4_HZ      349.23
#define G4_HZ      392.00
#define A4_HZ      440.00
#define B4_HZ      493.88
#define C5_HZ      523.25

// Define a C-major scale to play all the notes up and down.
float scale[MUSIC_NOTE_MAX] = { C4_HZ, D4_HZ, E4_HZ, F4_HZ, G4_HZ, A4_HZ, B4_HZ, C5_HZ };

// Store basic waveforms in memory.
int32_t sine[WAV_SIZE]     = {0};
int32_t sawtooth[WAV_SIZE] = {0};
int32_t triangle[WAV_SIZE] = {0};
int32_t square[WAV_SIZE]   = {0};


#define OUT_BUFSIZE (WAV_SIZE * 6)
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

CY_ISR(i2s_tx_dma_done)
{
  audioSetEvents(AUDIO_EVENTS_I2S_TX_DMA_DONE);
}

void generateSine(int32_t amplitude, int32_t* buffer, uint16_t length) {
  // Generate a sine wave signal with the provided amplitude and store it in
  // the provided buffer of size length.
  for (int i=0; i<length; ++i) {
    buffer[i] = (int32_t)((float)(amplitude)*sin(2.0*PI*(1.0/length)*i));
  }
}

void generateSineByFreq(float frequency, int32_t amplitude, int32_t *buffer, uint16_t *pLength)
{
  float newLength = (float)SAMPLERATE_HZ / frequency;
  
  *pLength = (uint16_t)newLength;

  // Generate a sine wave signal with the provided amplitude and store it in
  // the provided buffer of size length.
  for (int i=0; i<*pLength; ++i) {
    buffer[i] = (int32_t)((float)(amplitude)*sin(2.0*PI*(1.0/(*pLength))*i));
  }
}

void generateSawtooth(int32_t amplitude, int32_t* buffer, uint16_t length) {
  // Generate a sawtooth signal that goes from -amplitude/2 to amplitude/2
  // and store it in the provided buffer of size length.
  float delta = (float)(amplitude)/(float)(length);
  for (int i=0; i<length; ++i) {
    buffer[i] = -(amplitude/2)+delta*i;
  }
}

void generateTriangle(int32_t amplitude, int32_t* buffer, uint16_t length) {
  // Generate a triangle wave signal with the provided amplitude and store it in
  // the provided buffer of size length.
  float delta = (float)(amplitude)/(float)(length);
  for (int i=0; i<length/2; ++i) {
    buffer[i] = -(amplitude/2)+delta*i;
  }
    for (int i=length/2; i<length; ++i) {
    buffer[i] = (amplitude/2)-delta*(i-length/2);
  }
}

void generateSquare(int32_t amplitude, int32_t* buffer, uint16_t length) {
  // Generate a square wave signal with the provided amplitude and store it in
  // the provided buffer of size length.
  for (int i=0; i<length/2; ++i) {
    buffer[i] = -(amplitude/2);
  }
    for (int i=length/2; i<length; ++i) {
    buffer[i] = (amplitude/2);
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
	TxDMA_SetNumDataElements(0, OUT_BUFSIZE);
  TxDMA_SetSrcAddress(0, (void *) outBuffer);
	TxDMA_SetDstAddress(0, (void *) I2S_TX_FIFO_0_PTR);

  /* Validate descriptor */
  TxDMA_ValidateDescriptor(0);
  
  /* Start interrupts */
  isr_TxDMADone_StartEx(&i2s_tx_dma_done);
  
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
  
  I2S_Start();
  
  generateSine(AMPLITUDE, sine, WAV_SIZE);
  generateSawtooth(AMPLITUDE, sawtooth, WAV_SIZE);
  generateTriangle(AMPLITUDE, triangle, WAV_SIZE);
  generateSquare(AMPLITUDE, square, WAV_SIZE);
  
  
}
#if 1
void playWave(int32_t* buffer, uint16_t length, float frequency, int seconds)
{
  // Play back the provided waveform buffer for the specified
  // amount of seconds.
  // First calculate how many samples need to play back to run
  // for the desired amount of seconds.
  uint32_t iterations = seconds*SAMPLERATE_HZ;
  // Then calculate the 'speed' at which we move through the wave
  // buffer based on the frequency of the tone being played.
  float delta = (frequency*length)/(float)(SAMPLERATE_HZ);
  // Now loop through all the samples and play them, calculating the
  // position within the wave buffer for each moment in time.

  for (uint32_t i=0; i<iterations; ++i)
  {
    uint16_t pos = (uint32_t)(i*delta) % length;
    int32_t sample = buffer[pos];
    // Duplicate the sample so it's sent to both the left and right channel.
    // It appears the order is right channel, left channel if you want to write
    // stereo sound.
    //i2s.write(sample, sample);
    I2S_WriteByte(sample, 0);
    I2S_WriteByte(sample, 1);
  }
}
#endif

static void audioManagerPlayNote(music_note_e note, uint32_t durationMs)
{
  uint32_t i;
  int32_t sample;
  uint16_t length;
  
  if (note == MUSIC_NOTE_none)
  {
    CyDelay(durationMs);
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
  
  I2S_EnableTx(); /* Unmute the TX output */
  
  TxDMA_ChEnable();
  
  CyDelay(durationMs);
  
  I2S_DisableTx(); /* Mute the TX output */
  
  TxDMA_ChDisable();
}

static void audioManagerPlayPhrase(music_word_s *pPhrase)
{ 
  Codec_Activate();

  /* Enable power to speaker output */
  Codec_PowerOnControl(CODEC_POWER_CTRL_OUTPD);
  
  while (pPhrase->duration_ms)
  {
    audioManagerPlayNote(pPhrase->note, pPhrase->duration_ms);
    pPhrase++;
  }
  
  /* Enable power to speaker output */
  Codec_PowerOffControl(CODEC_POWER_CTRL_OUTPD);

  Codec_Deactivate();

  I2S_DisableTx();
}

void AudioManagerTonePlay(music_note_e note, uint32_t durationMs)
{  
  Codec_Activate();
  
  /* Enable power to speaker output */
  Codec_PowerOnControl(CODEC_POWER_CTRL_OUTPD);
  
  audioManagerPlayNote(note, durationMs);
  
  /* Enable power to speaker output */
  Codec_PowerOffControl(CODEC_POWER_CTRL_OUTPD);

  Codec_Deactivate();
}

void AudioManagerCuePlay(audio_cue_e cue)
{
  audioManagerPlayPhrase((music_word_s *)cues[cue]);
}

void AudioManagerService(void)
{
  if (audioCheckEvents(AUDIO_EVENTS_I2S_TX_DMA_DONE))
  {
    D_PRINTF(DEBUG, "I2S TX DMA done\n");
    audioClearEvents(AUDIO_EVENTS_I2S_TX_DMA_DONE);
  }
}

/* [] END OF FILE */
