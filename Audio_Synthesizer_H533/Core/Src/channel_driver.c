/**
 ******************************************************************************
 * @file    CHANNEL_TIMER.c
 * @brief   Channel1 Timer Control Interface
 * @author  Adrian Sucahyo
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 Synthetic Bits.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "channel_driver.h"

#include "audio_config.h"
#include "channel_common.h"
#include "config.h"
#include "rcc.h"

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32h5xx_hal.h"

#include "sine_base.h"
#include "trig_base.h"
#include "ramp_base.h"
#include "square_base.h"

#include "attack_base.h"
#include "decay_base.h"

#include "modulation_base.h"

#include "notes.h"

/* Function Prototypes -------------------------------------------------------*/

void channel_enable(channel_t channel);
void channel_disable(channel_t channel);
void channel_set_waveform(channel_t channel, waveforms_t wave);
void channel_volume(channel_t channel, uint8_t volume);

void channel_voice_on(channel_t channel, uint8_t voice);
void channel_voice_off(channel_t channel, uint8_t voice);
void channel_voice_frequency(channel_t channel, uint8_t voice, uint16_t freq);
void channel_voice_modulation(channel_t channel, uint8_t voice, uint16_t modulation);
void channel_voice_velocity(channel_t channel, uint8_t voice, uint8_t velocity);

static inline void channel_update_CCR(channel_t channel, uint32_t ccr);
static inline void channel_update(volatile channel_state_t *channel);
void channel_update_all();

static void reset_channel(volatile channel_state_t *channel, channel_t channel_num, uint8_t num_voices);
static void channel_timer_gpio_init();
void channel_timer_init();

/* ========================================================================== */
/*                                                                            */
/*    Local Variables Definitions                                             */
/*                                                                            */
/* ========================================================================== */

#define CHANNEL_TIMER_PSC (1 - 1)
#define CHANNEL_TIMER_ARR ((0x1 << 10) - 1) // ~240 kHz (244 kHz)

#define CHANNEL_SAMPLING_VS_RESOLUTION_DIFF (6) // 65536 -> 1024 (RSH 6)
#define CHANNEL_SAMPLING_VS_ENVELOPE_DIFF (4)   // 65536 -> 4096 (RSH 4)

volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state, channel5_state, channel6_state, channel7_state;

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

void channel_enable(channel_t channel)
{
  switch (channel)
  {
  case CHANNEL1:
    channel1_state.enable = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC1E;
    break;
  case CHANNEL2:
    channel2_state.enable = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC2E;
    break;
  case CHANNEL3:
    channel3_state.enable = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC3E;
    break;
  case CHANNEL4:
    channel4_state.enable = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC4E;
    break;
  case CHANNEL5:
    channel1_state.enable = 1;
    CHANNEL5_7_TIMER->CCER |= TIM_CCER_CC1E;
    break;
  case CHANNEL6:
    channel2_state.enable = 1;
    CHANNEL5_7_TIMER->CCER |= TIM_CCER_CC2E;
    break;
  case CHANNEL7:
    channel3_state.enable = 1;
    CHANNEL5_7_TIMER->CCER |= TIM_CCER_CC3E;
    break;
  default:
    return;
  }
}

void channel_disable(channel_t channel)
{
  switch (channel)
  {
  case CHANNEL1:
    channel1_state.enable = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC1E;
    break;
  case CHANNEL2:
    channel2_state.enable = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC2E;
    break;
  case CHANNEL3:
    channel3_state.enable = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC3E;
    break;
  case CHANNEL4:
    channel4_state.enable = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC4E;
    break;
  case CHANNEL5:
    channel1_state.enable = 0;
    CHANNEL5_7_TIMER->CCER &= ~TIM_CCER_CC1E;
    break;
  case CHANNEL6:
    channel2_state.enable = 0;
    CHANNEL5_7_TIMER->CCER &= ~TIM_CCER_CC2E;
    break;
  case CHANNEL7:
    channel3_state.enable = 0;
    CHANNEL5_7_TIMER->CCER &= ~TIM_CCER_CC3E;
    break;
  default:
    return;
  }
}

void channel_set_waveform(channel_t channel, waveforms_t wave)
{
  const unsigned short *curr_wave;

  switch (wave)
  {
  case WAVEFORM_SINE:
    curr_wave = sine_base;
    break;
  case WAVEFORM_TRIG:
    curr_wave = trig_base;
    break;
  case WAVEFORM_RAMP:
    curr_wave = ramp_base;
    break;
  case WAVEFORM_SQUARE:
    curr_wave = square_base;
    break;
  default:
    return;
  }

  switch (channel)
  {
  case CHANNEL1:
    channel1_state.waveform = wave;
    channel1_state.waveform_data = curr_wave;
    break;
  case CHANNEL2:
    channel2_state.waveform = wave;
    channel2_state.waveform_data = curr_wave;
    break;
  case CHANNEL3:
    channel3_state.waveform = wave;
    channel3_state.waveform_data = curr_wave;
    break;
  case CHANNEL4:
    channel4_state.waveform = wave;
    channel4_state.waveform_data = curr_wave;
    break;
  case CHANNEL5:
    channel5_state.waveform = wave;
    channel5_state.waveform_data = curr_wave;
    break;
  case CHANNEL6:
    channel6_state.waveform = wave;
    channel6_state.waveform_data = curr_wave;
    break;
  case CHANNEL7:
    channel7_state.waveform = wave;
    channel7_state.waveform_data = curr_wave;
    break;
  default:
    return;
  }
}

void channel_volume(channel_t channel, uint8_t volume)
{
  switch (channel)
  {
  case CHANNEL1:
    channel1_state.volume = volume;
    break;
  case CHANNEL2:
    channel2_state.volume = volume;
    break;
  case CHANNEL3:
    channel3_state.volume = volume;
    break;
  case CHANNEL4:
    channel4_state.volume = volume;
    break;
  case CHANNEL5:
    channel5_state.volume = volume;
    break;
  case CHANNEL6:
    channel6_state.volume = volume;
    break;
  case CHANNEL7:
    channel7_state.volume = volume;
    break;
  default:
    return;
  }
}

void channel_voice_on(channel_t channel, uint8_t voice)
{
  if (voice >= MAX_CHANNEL_VOICES)
    return;

  switch (channel)
  {
  case CHANNEL1:
    channel1_state.voices[voice].on_off = 1;
    channel1_state.voices[voice].count = 0; // Reset the counter when starting a new tone
    channel1_state.voices[voice].mod_count = 0;
    channel1_state.voices[voice].env_count = 0;
    channel1_state.voices[voice].env_start = 0;
    break;
  case CHANNEL2:
    channel2_state.voices[voice].on_off = 1;
    channel2_state.voices[voice].count = 0; // Reset the counter when starting a new tone
    channel2_state.voices[voice].mod_count = 0;
    channel2_state.voices[voice].env_count = 0;
    break;
  case CHANNEL3:
    channel3_state.voices[voice].on_off = 1;
    channel3_state.voices[voice].count = 0; // Reset the counter when starting a new tone
    channel3_state.voices[voice].mod_count = 0;
    channel3_state.voices[voice].env_count = 0;
    break;
  case CHANNEL4:
    channel4_state.voices[voice].on_off = 1;
    channel4_state.voices[voice].count = 0; // Reset the counter when starting a new tone
    channel4_state.voices[voice].mod_count = 0;
    channel4_state.voices[voice].env_count = 0;
    break;
  case CHANNEL5:
    channel5_state.voices[voice].on_off = 1;
    channel5_state.voices[voice].count = 0; // Reset the counter when starting a new tone
    channel5_state.voices[voice].mod_count = 0;
    channel5_state.voices[voice].env_count = 0;
    break;
  case CHANNEL6:
    channel6_state.voices[voice].on_off = 1;
    channel6_state.voices[voice].count = 0; // Reset the counter when starting a new tone
    channel6_state.voices[voice].mod_count = 0;
    channel6_state.voices[voice].env_count = 0;
    break;
  case CHANNEL7:
    channel7_state.voices[voice].on_off = 1;
    channel7_state.voices[voice].count = 0; // Reset the counter when starting a new tone
    channel7_state.voices[voice].mod_count = 0;
    channel7_state.voices[voice].env_count = 0;
    break;
  default:
    return;
  }
}

void channel_voice_off(channel_t channel, uint8_t voice)
{
  if (voice >= MAX_CHANNEL_VOICES)
    return;

  switch (channel)
  {
  case CHANNEL1:
    channel1_state.voices[voice].on_off = 0;
    break;
  case CHANNEL2:
    channel2_state.voices[voice].on_off = 0;
    break;
  case CHANNEL3:
    channel3_state.voices[voice].on_off = 0;
    break;
  case CHANNEL4:
    channel4_state.voices[voice].on_off = 0;
    break;
  case CHANNEL5:
    channel5_state.voices[voice].on_off = 0;
    break;
  case CHANNEL6:
    channel6_state.voices[voice].on_off = 0;
    break;
  case CHANNEL7:
    channel7_state.voices[voice].on_off = 0;
    break;
  default:
    return;
  }
}

void channel_voice_frequency(channel_t channel, uint8_t voice, uint16_t freq)
{
  if (voice >= MAX_CHANNEL_VOICES)
    return;

  switch (channel)
  {
  case CHANNEL1:
    channel1_state.voices[voice].frequency = freq;
    break;
  case CHANNEL2:
    channel2_state.voices[voice].frequency = freq;
    break;
  case CHANNEL3:
    channel3_state.voices[voice].frequency = freq;
    break;
  case CHANNEL4:
    channel4_state.voices[voice].frequency = freq;
    break;
  case CHANNEL5:
    channel5_state.voices[voice].frequency = freq;
    break;
  case CHANNEL6:
    channel6_state.voices[voice].frequency = freq;
    break;
  case CHANNEL7:
    channel7_state.voices[voice].frequency = freq;
    break;
  default:
    return;
  }
}

void channel_voice_modulation(channel_t channel, uint8_t voice, uint16_t modulation)
{
  if (voice >= MAX_CHANNEL_VOICES)
    return;

  switch (channel)
  {
  case CHANNEL1:
    channel1_state.voices[voice].mod = modulation;
    break;
  case CHANNEL2:
    channel2_state.voices[voice].mod = modulation;
    break;
  case CHANNEL3:
    channel3_state.voices[voice].mod = modulation;
    break;
  case CHANNEL4:
    channel4_state.voices[voice].mod = modulation;
    break;
  case CHANNEL5:
    channel5_state.voices[voice].mod = modulation;
    break;
  case CHANNEL6:
    channel6_state.voices[voice].mod = modulation;
    break;
  case CHANNEL7:
    channel7_state.voices[voice].mod = modulation;
    break;
  default:
    return;
  }
}

void channel_voice_velocity(channel_t channel, uint8_t voice, uint8_t velocity)
{
  if (voice >= MAX_CHANNEL_VOICES)
    return;

  switch (channel)
  {
  case CHANNEL1:
    channel1_state.voices[voice].mod = velocity;
    break;
  case CHANNEL2:
    channel2_state.voices[voice].mod = velocity;
    break;
  case CHANNEL3:
    channel3_state.voices[voice].mod = velocity;
    break;
  case CHANNEL4:
    channel4_state.voices[voice].mod = velocity;
    break;
  case CHANNEL5:
    channel5_state.voices[voice].mod = velocity;
    break;
  case CHANNEL6:
    channel6_state.voices[voice].mod = velocity;
    break;
  case CHANNEL7:
    channel7_state.voices[voice].mod = velocity;
    break;
  default:
    return;
  }
}

// ========= PRIVATE UPDATE FUNCTIONS =========

/**
 * @brief Update the CCR (capture and control register) in the timer channel (PWM)
 * @param channel channel to update
 * @param ccr CCR value to write
 */
static inline void channel_update_CCR(channel_t channel, uint32_t ccr)
{
  switch (channel)
  {
  case CHANNEL1:
    CHANNEL1_4_TIMER->CCR4 = ccr;
    break;
  case CHANNEL2:
    CHANNEL1_4_TIMER->CCR1 = ccr;
    break;
  case CHANNEL3:
    CHANNEL1_4_TIMER->CCR2 = ccr;
    break;
  case CHANNEL4:
    CHANNEL1_4_TIMER->CCR3 = ccr;
    break;
  case CHANNEL5:
    CHANNEL5_7_TIMER->CCR1 = ccr;
    break;
  case CHANNEL6:
    CHANNEL5_7_TIMER->CCR2 = ccr;
    break;
  case CHANNEL7:
    CHANNEL5_7_TIMER->CCR3 = ccr;
    break;
  default:
    return;
  }
}

static inline uint32_t calculate_voice_output(volatile channel_state_t *channel, uint8_t voice)
{
  voice_t cur_voice = channel->voices[voice];

  if ((!cur_voice.on_off) && cur_voice.adsr_state != ADSR_ATTACK)
    return 0;

  uint32_t output, envelope = 0;
  int32_t frequency_delta = 0;

  // === Find the current state of the envelope ===
  // Check the ADSR
  switch (cur_voice.adsr_state)
  {
  case ADSR_ATTACK:
    channel1_state.voices[voice].env_target = (ADSR_RANGE * 1016 * (cur_voice.velocity)) + ADSR_MIN; // Calculate the target endpoint

    cur_voice.env_count++; // Increment the current envelope counter

    // Advance to next stage if complete
    if (cur_voice.env_count > SAMPLE_RESOLUTION_MASK)
    {
      cur_voice.adsr_state = ADSR_DECAY;
      cur_voice.env_count = 0;
      cur_voice.env_start = channel1_state.voices[voice].env_target;
      break;
    }

    // Calculate the envelope (and scale: 10 bits + 10 bits + 7 bits - 17 bits)
    envelope = ((ADSR_RANGE * attack_base[cur_voice.env_count >> CHANNEL_SAMPLING_VS_ENVELOPE_DIFF] * (cur_voice.velocity)) >> 17) + ADSR_MIN;

    break;
  case ADSR_DECAY:
    channel1_state.voices[voice].env_target = ADSR_MID;

    cur_voice.env_count++; // Increment the current envelope counter

    // Advance to next stage if complete
    if (cur_voice.env_count > SAMPLE_RESOLUTION_MASK)
    {
      cur_voice.adsr_state = ADSR_SUSTAIN;
      cur_voice.env_count = 0;
      cur_voice.env_start = channel1_state.voices[voice].env_target;
      break;
    }

    // Calculate the envelope (and scale: 10 bits + 10 bits - 10 bits)
    envelope = ((((int32_t)(cur_voice.env_start - channel1_state.voices[voice].env_target))) * decay_base[cur_voice.env_count >> CHANNEL_SAMPLING_VS_ENVELOPE_DIFF] >> 10) + ADSR_MID;

    break;
  case ADSR_SUSTAIN:
    break;
  case ADSR_RELEASE:
    cur_voice.env_count++; // Increment the current envelope counter

    // Advance to next stage if complete
    if (cur_voice.env_count > SAMPLE_RESOLUTION_MASK)
    {
      cur_voice.adsr_state = ADSR_ATTACK;
      cur_voice.env_count = 0;
      cur_voice.env_start = channel1_state.voices[voice].env_target;
      break;
    }

    // Calculate the envelope (and scale: 10 bits + 10 bits - 10 bits)
    envelope = (cur_voice.env_start * decay_base[cur_voice.env_count >> CHANNEL_SAMPLING_VS_ENVELOPE_DIFF] >> 10);
    break;
  }

  // === Generate the current state of the waveform ===
  // Check if pitch bend needs calculation
  if (cur_voice.pitch_bend != (0x1 << 6))
  {
    // Add the pitch bend (calculate the +2 semitone and scale - scaled by: 9 bits + 6 bits - 15 bits)
    frequency_delta += ((cur_voice.frequency) * TWO_SEMITONE_SCALAR * ((cur_voice.pitch_bend >> 7) - (0x1 << 6))) >> (15);
  }

  // Check if modulation needs calculation
  if (cur_voice.mod)
  {
    // Advance the modulation coutner
    cur_voice.mod_count++;
    cur_voice.mod_count &= SAMPLE_FREQUENCY_MASK;

    // Add the modulation (calculate the +2 semitone and scale - scaled by: 9 bits + 9 bits + 8 bits - 16 bits - 10 bits)
    frequency_delta += ((cur_voice.frequency) * (((modulation_base[cur_voice.mod_count >> CHANNEL_SAMPLING_VS_RESOLUTION_DIFF]) * TWO_SEMITONE_SCALAR * (cur_voice.mod >> 7)) >> (16))) >> 10;
  }

  // Advance the lookup counter (and mask (faster modulo))
  cur_voice.count += (cur_voice.frequency + frequency_delta);
  cur_voice.count &= SAMPLE_FREQUENCY_MASK;

  output = channel->waveform_data[cur_voice.count >> CHANNEL_SAMPLING_VS_RESOLUTION_DIFF] * envelope;

  return (output >> 10);
}

static inline void channel_update(volatile channel_state_t *channel)
{
  if (!channel->enable) // Don't calculate if the channel is disabled
    return;

  uint32_t output = 0;

  for (uint8_t i = 0; i < channel->num_voices; i++)
  {
    output += calculate_voice_output(channel, i);
  }

  // Scale all the channels down
  uint16_t ccr = output / channel->active_voices;

  // Update the resultant value in the CCR (modulate timer PWM)
  channel_update_CCR(channel->channel, ccr);
}

void channel_update_all()
{
  channel_update(&channel1_state);
  channel_update(&channel2_state);
  channel_update(&channel3_state);
  channel_update(&channel4_state);
  channel_update(&channel5_state);
  channel_update(&channel6_state);
  channel_update(&channel7_state);
}

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

static void reset_channel(volatile channel_state_t *channel, channel_t channel_num, uint8_t num_voices)
{
  // Reset Channel Parameters
  channel->enable = 0;
  channel->volume = MIDI_MAX_VAL;
  channel->waveform_data = sine_base;
  channel->channel = channel_num;
  channel->active_voices = 0;

  if (num_voices > MAX_CHANNEL_VOICES)
    channel->num_voices = MAX_CHANNEL_VOICES;
  else
    channel->num_voices = num_voices;

  // Reset all voice parameters
  for (uint8_t i = 0; i < num_voices; i++)
  {
    channel->voices[i].count = 0;
    channel->voices[i].frequency = 0;
    channel->voices[i].pitch_bend = 0;
    channel->voices[i].velocity = 64; // (50%)
    channel->voices[i].on_off = 0;
    channel->voices[i].adsr_state = ADSR_ATTACK;
  }
}

static void channel_timer_gpio_init()
{
  RCC_GPIOA_CLK_Enable();
  RCC_GPIOB_CLK_Enable();
  RCC_GPIOC_CLK_Enable();

  GPIO_InitTypeDef initChannel1_4 = {
      CHANNEL1_GPIO_PIN | CHANNEL2_GPIO_PIN | CHANNEL3_GPIO_PIN | CHANNEL4_GPIO_PIN,
      GPIO_MODE_AF_PP,
      GPIO_NOPULL,
      GPIO_SPEED_FREQ_HIGH,
      GPIO_AF2_TIM3};

  HAL_GPIO_Init(CHANNEL1_4_GPIO_PORT, &initChannel1_4);

  GPIO_InitTypeDef initChannel5_7 = {
      CHANNEL5_GPIO_PIN | CHANNEL6_GPIO_PIN | CHANNEL7_GPIO_PIN,
      GPIO_MODE_AF_PP,
      GPIO_NOPULL,
      GPIO_SPEED_FREQ_HIGH,
      GPIO_AF2_TIM4};

  HAL_GPIO_Init(CHANNEL5_7_GPIO_PORT, &initChannel5_7);
}

void channel_timer_init()
{
  // Enable the RCC for the Timer
  RCC_TIM3_CLK_Enable();
  RCC_TIM4_CLK_Enable();

  channel_timer_gpio_init();

  // Reset all channel structs
  reset_channel(&channel1_state, CHANNEL1, 4);
  reset_channel(&channel2_state, CHANNEL2, 4);
  reset_channel(&channel3_state, CHANNEL3, 4);
  reset_channel(&channel4_state, CHANNEL4, 4);
  reset_channel(&channel5_state, CHANNEL5, 1);
  reset_channel(&channel6_state, CHANNEL6, 1);
  reset_channel(&channel7_state, CHANNEL7, 1);

  // Configure Channels 1-4
  CHANNEL1_4_TIMER->PSC = CHANNEL_TIMER_PSC;
  CHANNEL1_4_TIMER->ARR = CHANNEL_TIMER_ARR;

  CHANNEL1_4_TIMER->CCMR1 &= ~TIM_CCMR1_CC1S;                                                                      // Select CCS Mode 0 - CC1 (output)
  CHANNEL1_4_TIMER->CCMR1 = ((~TIM_CCMR1_OC1M) & CHANNEL1_4_TIMER->CCMR1) | (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // Enable PWM Mode 1 - OC1M
  CHANNEL1_4_TIMER->CCMR1 = ((~TIM_CCMR1_OC1CE) & CHANNEL1_4_TIMER->CCMR1) | TIM_CCMR1_OC1CE;                      // Enable Clear - OC1C
  CHANNEL1_4_TIMER->CCMR1 &= ~TIM_CCMR1_CC2S;                                                                      // Select CCS Mode 0 - CC2(output)
  CHANNEL1_4_TIMER->CCMR1 = ((~TIM_CCMR1_OC2M) & CHANNEL1_4_TIMER->CCMR1) | (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); // Enable PWM Mode 1 - OC2M
  CHANNEL1_4_TIMER->CCMR1 = ((~TIM_CCMR1_OC2CE) & CHANNEL1_4_TIMER->CCMR1) | TIM_CCMR1_OC2CE;                      // Enable Clear - OC2C

  CHANNEL1_4_TIMER->CCMR2 &= ~TIM_CCMR2_CC3S;                                                                      // Select CCS Mode 0 - CC3 (output)
  CHANNEL1_4_TIMER->CCMR2 = ((~TIM_CCMR2_OC3M) & CHANNEL1_4_TIMER->CCMR2) | (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // Enable PWM Mode 1 - OC3M
  CHANNEL1_4_TIMER->CCMR2 = ((~TIM_CCMR2_OC3CE) & CHANNEL1_4_TIMER->CCMR2) | TIM_CCMR2_OC3CE;                      // Enable Clear - OC3C
  CHANNEL1_4_TIMER->CCMR2 &= ~TIM_CCMR2_CC4S;                                                                      // Select CCS Mode 0 - CC4(output)
  CHANNEL1_4_TIMER->CCMR2 = ((~TIM_CCMR2_OC4M) & CHANNEL1_4_TIMER->CCMR2) | (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // Enable PWM Mode 1 - OC4M
  CHANNEL1_4_TIMER->CCMR2 = ((~TIM_CCMR2_OC4CE) & CHANNEL1_4_TIMER->CCMR2) | TIM_CCMR2_OC4CE;                      // Enable Clear - OC4C

  CHANNEL1_4_TIMER->CCR1 = (CHANNEL_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL1_4_TIMER->CCR2 = (CHANNEL_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL1_4_TIMER->CCR3 = (CHANNEL_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL1_4_TIMER->CCR4 = (CHANNEL_TIMER_ARR >> 0x1); // Set default duty cycle to 50%

  // Configure Channels 5-7
  CHANNEL5_7_TIMER->PSC = CHANNEL_TIMER_PSC;
  CHANNEL5_7_TIMER->ARR = CHANNEL_TIMER_ARR;

  CHANNEL5_7_TIMER->CCMR1 &= ~TIM_CCMR1_CC1S;                                                                      // Select CCS Mode 0 - CC1 (output)
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC1M) & CHANNEL5_7_TIMER->CCMR1) | (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // Enable PWM Mode 1 - OC1M
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC1CE) & CHANNEL5_7_TIMER->CCMR1) | TIM_CCMR1_OC1CE;                      // Enable Clear - OC1C
  CHANNEL5_7_TIMER->CCMR1 &= ~TIM_CCMR1_CC2S;                                                                      // Select CCS Mode 0 - CC2(output)
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC2M) & CHANNEL5_7_TIMER->CCMR1) | (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); // Enable PWM Mode 1 - OC2M
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC2CE) & CHANNEL5_7_TIMER->CCMR1) | TIM_CCMR1_OC2CE;                      // Enable Clear - OC2C

  CHANNEL5_7_TIMER->CCMR2 &= ~TIM_CCMR2_CC3S;                                                                      // Select CCS Mode 0 - CC3 (output)
  CHANNEL5_7_TIMER->CCMR2 = ((~TIM_CCMR2_OC3M) & CHANNEL5_7_TIMER->CCMR2) | (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // Enable PWM Mode 1 - OC3M
  CHANNEL5_7_TIMER->CCMR2 = ((~TIM_CCMR2_OC3CE) & CHANNEL5_7_TIMER->CCMR2) | TIM_CCMR2_OC3CE;                      // Enable Clear - OC3C

  CHANNEL5_7_TIMER->CCR1 = (CHANNEL_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL5_7_TIMER->CCR2 = (CHANNEL_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL5_7_TIMER->CCR3 = (CHANNEL_TIMER_ARR >> 0x1); // Set default duty cycle to 50%

  // Start the Timers for all Channels
  CHANNEL1_4_TIMER->CR1 |= TIM_CR1_CEN; // Start the Timer
  CHANNEL5_7_TIMER->CR1 |= TIM_CR1_CEN; // Start the Timer
}