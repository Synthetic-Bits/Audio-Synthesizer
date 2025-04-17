/**
 ******************************************************************************
 * @file           : channel_common.h
 * @brief          : Common Channel Component Defintions
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/* ========================================================================== */
/*                                                                            */
/*    Channel Definitions                                                     */
/*                                                                            */
/* ========================================================================== */

#ifndef _CHANNEL_COMMON_H_
#define _CHANNEL_COMMON_H_

#define MAX_CHANNEL_VOICES 4

#define ADSR_MIN (uint16_t)(0.4 * 1024)
#define ADSR_MID (uint16_t)(0.7 * 1024)
#define ADSR_MAX (uint16_t)(1024)
#define ADSR_RANGE (ADSR_MAX - ADSR_MIN)

// Define the channels available to control
typedef enum
{
  CHANNEL1,
  CHANNEL2,
  CHANNEL3,
  CHANNEL4,
  CHANNEL5,
  CHANNEL6,
  CHANNEL7,
  // CHANNEL8 - TODO NOISE CHANNEL
} channel_t;

// Define the types of waveforms available
typedef enum
{
  WAVEFORM_SINE,
  WAVEFORM_TRIG,
  WAVEFORM_RAMP,
  WAVEFORM_SQUARE,
  WAVEFORM_NOISE
} waveforms_t;

// Define the ADSR envelopes
typedef enum
{
  ADSR_ATTACK,
  ADSR_DECAY,
  ADSR_SUSTAIN,
  ADSR_RELEASE
} adsr_t;

// Define the voice structs
typedef struct
{
  // Waveform Calculations
  uint32_t count;      // Internal counter for the voice waveform
  uint32_t frequency;  // Current frequency being played
  uint16_t pitch_bend; // Pitch bend from the baseline frequency (only use MSBs if from 14 bit command)
  uint32_t mod_count;  // Internal counter for the modulator
  uint16_t mod;        // Intensity of the modulation
  uint8_t on_off;      // Turn this voice on or off

  // Envelope Calculations
  adsr_t adsr_state;   // Current state of the ADSR
  uint32_t env_count;  // Internal counter for the voice envelope
  uint32_t env;        // Current envelope scalar value for this voice
  uint16_t env_start;  // Starting value of the envelope
  uint16_t env_target; // Target value of the envelope
  uint8_t velocity;    // Velocity of the ASDR response
} voice_t;

// Define channel structs
typedef struct
{
  uint8_t enable;       // Enable / Disable updates to this channel
  channel_t channel;    // Self Identify this struct
  waveforms_t waveform; // Waveform for this channel
  uint16_t volume;      // Volume of the channel

  uint8_t num_voices;                 // Number of voices on the channel
  uint8_t active_voices;              // Number of active voices
  voice_t voices[MAX_CHANNEL_VOICES]; // Voices for the channel

  const unsigned short *waveform_data; // Pointer to the waveform data LUT
} channel_state_t;

#endif /* _CHANNEL_COMMON_H_ */