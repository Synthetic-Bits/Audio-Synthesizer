/**
 *******************************************************************************
 * @file    noise_channel.c
 * @brief   DAC-based Noise Channel Control Interface Source File
 * @attention
 *
 * This file implements the noise channel functionality using a DAC.
 * The noise waveform data is taken from noise_base.h.
 *
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "noise_channel.h"
#include "audio_config.h"
#include "channel_common.h"
#include "config.h"
#include "rcc.h"

#include "stm32h5xx_hal.h"
#include "noise_base.h"  // Noise sample data

/* Private Variables ---------------------------------------------------------*/

/* Noise channel state structure */
typedef struct {
    uint32_t count;       // Sample index
    uint16_t freq;        // Index increment value (controls output frequency)
    uint8_t vol;          // Volume (0~127)
    uint8_t on_off;       // Channel on/off status (1: on, 0: off)
    waveforms_t waveform; // Current waveform (should be WAVEFORM_NOISE)
} noise_channel_state_t;

static noise_channel_state_t noise_state;

/* DAC handle (configured in CubeMX or other initialization code) */
extern DAC_HandleTypeDef hdac1;  // Using DAC1, for example

/* Private Function Prototypes ---------------------------------------------*/
static void reset_noise_channel(void);

/* Exported Functions --------------------------------------------------------*/

/**
 * @brief Enable the noise channel output via DAC.
 * @note  Starts the DAC conversion on DAC_CHANNEL_1.
 */
void noise_channel_enable(void)
{
  if (HAL_DAC_Start(&hdac1, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief Disable the noise channel output.
 * @note  Stops the DAC conversion on DAC_CHANNEL_1.
 */
void noise_channel_disable(void)
{
  if (HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief Set the waveform for the noise channel.
 * @param wave The waveform to use (must be WAVEFORM_NOISE).
 */
void noise_channel_set_waveform(waveforms_t wave)
{
  if (wave != WAVEFORM_NOISE)
    return;  // Only WAVEFORM_NOISE is supported for the noise channel
  
  noise_state.waveform = wave;
  // Using the noise_base array, so no extra allocation is required.
}

/**
 * @brief Turn the noise channel on or off.
 * @param state 1 to turn on, 0 to turn off.
 * @note  Resets the sample counter when turning on.
 */
void noise_channel_on_off(uint8_t state)
{
  if (state)
  {
    noise_state.on_off = 1;
    noise_state.count = 0;
  }
  else
  {
    noise_state.on_off = 0;
  }
}

/**
 * @brief Set the noise channel volume.
 * @param volume Volume level (0 ~ 127).
 */
void noise_channel_volume(uint8_t volume)
{
  noise_state.vol = volume;
}

/**
 * @brief Set the noise channel frequency.
 * @param freq Frequency parameter affecting the sample index increment.
 */
void noise_channel_frequency(uint16_t freq)
{
  noise_state.freq = freq;
}

/**
 * @brief Update the DAC output based on the noise channel state.
 * @note  This function should be called periodically to update the DAC output.
 */
void noise_channel_update(void)
{
  if (!noise_state.on_off)
  {
    // If the channel is off, set the DAC output to the mid-level value (e.g., 2048 for 12-bit DAC)
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
    return;
  }

  // Increment sample counter
  noise_state.count += noise_state.freq;
  while (noise_state.count >= SAMPLE_FREQUENCY)
  {
    noise_state.count -= SAMPLE_FREQUENCY;
  }

  // Get the sample from noise_base array (assumed to have 65536 samples)
  uint16_t sample = noise_base[noise_state.count];

  // Adjust sample value based on volume (lower volume reduces the signal amplitude)
  uint8_t shift = ((MIDI_MAX_VAL - noise_state.vol) >> 4);
  sample = sample >> shift;

  // Update DAC output (assuming a 12-bit DAC)
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sample);
}

/**
 * @brief Initialize the DAC-based noise channel.
 */
void noise_channel_init(void)
{
  reset_noise_channel();
  // DAC initialization (HAL_DAC_Init()) is assumed to be handled elsewhere (e.g., CubeMX)
}

/* Private Functions ---------------------------------------------------------*/

/**
 * @brief Reset the noise channel state.
 */
static void reset_noise_channel(void)
{
  noise_state.count = 0;
  noise_state.on_off = 0;
  noise_state.freq = 0;
  noise_state.vol = MIDI_MAX_VAL;  // Set volume to maximum
  noise_state.waveform = WAVEFORM_NOISE;
}
