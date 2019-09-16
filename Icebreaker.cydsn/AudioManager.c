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

// Define a C-major scale to play all the notes up and down.
float scale[] = { C4_HZ, D4_HZ, E4_HZ, F4_HZ, G4_HZ, A4_HZ, B4_HZ, A4_HZ, G4_HZ, F4_HZ, E4_HZ, D4_HZ, C4_HZ };

// Store basic waveforms in memory.
int32_t sine[WAV_SIZE]     = {0};
int32_t sawtooth[WAV_SIZE] = {0};
int32_t triangle[WAV_SIZE] = {0};
int32_t square[WAV_SIZE]   = {0};


#define OUT_BUFSIZE (WAV_SIZE * 6)
uint8 outBuffer[OUT_BUFSIZE];

CY_ISR(i2s_tx_dma_done)
{
  audioSetEvents(AUDIO_EVENTS_I2S_TX_DMA_DONE);
}

void generateSine(int32_t amplitude, int32_t* buffer, uint16_t length) {
  // Generate a sine wave signal with the provided amplitude and store it in
  // the provided buffer of size length.
  for (int i=0; i<length; ++i) {
//    buffer[i] = (int32_t)((float)(amplitude)*sin(2.0*PI*(1.0/length)*i));
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

void AudioManagerToneStart(uint32_t word)
{
#if 0
  int i;
  uint32_t word1 = 0x8899AABB;
  uint32_t word2 = 0xCCDDEEFF;

  for (i = 0; i < OUT_BUFSIZE; i += 2*sizeof(int32_t))
  {
    outBuffer[i+7] = word2 & 0xFF;         // "FF"
    outBuffer[i+6] = (word2 >> 8) & 0xFF;  // "EE"
    outBuffer[i+5] = (word2 >> 16) & 0xFF; // "DD"
    outBuffer[i+4] = (word2 >> 24) & 0xFF; // "CC"

    outBuffer[i+3] = word1 & 0xFF;         // "BB"
    outBuffer[i+2] = (word1 >> 8) & 0xFF;  // "AA"
    outBuffer[i+1] = (word1 >> 16) & 0xFF; // "99"
    outBuffer[i+0] = (word1 >> 24) & 0xFF; // "88"
    
  }
#else
  uint32_t i;
  uint16_t pos;
  //int32_t sample;
  float delta = (scale[word] * WAV_SIZE) / (float)(SAMPLERATE_HZ);
  
  for (i = 0; i < OUT_BUFSIZE; i += 6)
  {
    pos = (uint32_t)(i * delta) % WAV_SIZE;
    
    //memcpy(&outBuffer[i*4], &triangle[pos], sizeof(uint32_t));
    outBuffer[i+5] = (triangle[pos] >> 16) & 0xFF;
    outBuffer[i+4] = (triangle[pos] >> 8) & 0xFF;
    outBuffer[i+3] = triangle[pos] & 0xFF;
    
    outBuffer[i+2] = (triangle[pos] >> 16) & 0xFF;
    outBuffer[i+1] = (triangle[pos] >> 8) & 0xFF;
    outBuffer[i+0] = triangle[pos] & 0xFF;
    //outBuffer[i] = triangle[pos];
  }
#endif


  I2S_ClearTxFIFO(); /* Clear the I2S internal FIFO */

  //TxDMA_ChEnable();
  
  I2S_EnableTx(); /* Unmute the TX output */
  
  Codec_Activate();

  /* Enable power to speaker output */
  Codec_PowerOnControl(CODEC_POWER_CTRL_OUTPD);
  
  playWave((int32_t *)triangle, sizeof(triangle) / 4, C4_HZ, 10);
}

void AudioManagerToneStop(void)
{
  /* Enable power to speaker output */
  Codec_PowerOffControl(CODEC_POWER_CTRL_OUTPD);

  Codec_Deactivate();

  I2S_DisableTx();
  
  CyDelay(20);

  TxDMA_ChDisable();
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
