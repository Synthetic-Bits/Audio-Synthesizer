/**
 ******************************************************************************
 * @file    CHANNEL1_4_TIMER.c
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

#include "channel1_4_timer.h"

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

/* Function Prototypes -------------------------------------------------------*/

void channel1_4_enable(channel_t channel);
void channel1_4_disable(channel_t channel);
void channel1_4_set_waveform(channel_t channel, waveforms_t wave);
void channel1_4_on_off(channel_t channel, uint8_t state);
void channel1_4_volume(channel_t channel, uint8_t volume);
void channel1_4_frequency(channel_t channel, uint16_t freq);

static inline void channel_update_CCR(channel_t channel, uint32_t ccr);
static inline void channel_update(volatile channel_state_t *channel);
void channel1_4_update();

static void reset_channel(volatile channel_state_t *channel, channel_t channel_num);
static void channel1_4_timer_gpio_init();
void channel1_4_timer_init();

/* ========================================================================== */
/*                                                                            */
/*    Local Variables Definitions                                             */
/*                                                                            */
/* ========================================================================== */

#define CHANNEL1_4_TIMER_PSC (1 - 1)
#define CHANNEL1_4_TIMER_ARR ((0x1 << 10) - 1) // ~240 kHz (244 kHz)

volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state;

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

void channel1_4_enable(channel_t channel)
{
  switch (channel)
  {
  case CHANNEL1:
    channel1_state.enabled = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC1E;
    break;
  case CHANNEL2:
    channel2_state.enabled = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC2E;
    break;
  case CHANNEL3:
    channel3_state.enabled = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC3E;
    break;
  case CHANNEL4:
    channel4_state.enabled = 1;
    CHANNEL1_4_TIMER->CCER |= TIM_CCER_CC4E;
    break;
  default:
    return;
  }
}
void channel1_4_disable(channel_t channel)
{
  switch (channel)
  {
  case CHANNEL1:
    channel1_state.enabled = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC1E;
    break;
  case CHANNEL2:
    channel2_state.enabled = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC2E;
    break;
  case CHANNEL3:
    channel3_state.enabled = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC3E;
    break;
  case CHANNEL4:
    channel4_state.enabled = 0;
    CHANNEL1_4_TIMER->CCER &= ~TIM_CCER_CC4E;
    break;
  default:
    return;
  }
}

void channel1_4_set_waveform(channel_t channel, waveforms_t wave)
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
    curr_wave = sine_base; // Waveform is calculated
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
  default:
    return;
  }
}

void channel1_4_on_off(channel_t channel, uint8_t state)
{
  switch (channel)
  {
  case CHANNEL1:
    if (state)
    {
      channel1_state.on_off = 1;
      channel1_state.count = 0; // Reset the counter when starting a new tone
    }
    else
      channel1_state.on_off = 0;
    break;
  case CHANNEL2:
    if (state)
    {
      channel2_state.on_off = 1;
      channel2_state.count = 0; // Reset the counter when starting a new tone
    }
    else
      channel2_state.on_off = 0;
    break;
  case CHANNEL3:
    if (state)
    {
      channel3_state.on_off = 1;
      channel3_state.count = 0; // Reset the counter when starting a new tone
    }
    else
      channel3_state.on_off = 0;
    break;
  case CHANNEL4:
    if (state)
    {
      channel4_state.on_off = 1;
      channel4_state.count = 0; // Reset the counter when starting a new tone
    }
    else
      channel4_state.on_off = 0;
    break;
  default:
    return;
  }
}

void channel1_4_volume(channel_t channel, uint8_t volume)
{
  switch (channel)
  {
  case CHANNEL1:
    channel1_state.vol = volume;
    break;
  case CHANNEL2:
    channel2_state.vol = volume;
    break;
  case CHANNEL3:
    channel3_state.vol = volume;
    break;
  case CHANNEL4:
    channel4_state.vol = volume;
    break;
  default:
    return;
  }
}

void channel1_4_frequency(channel_t channel, uint16_t freq)
{
  switch (channel)
  {
  case CHANNEL1:
    channel1_state.freq = freq;
    break;
  case CHANNEL2:
    channel2_state.freq = freq;
    break;
  case CHANNEL3:
    channel3_state.freq = freq;
    break;
  case CHANNEL4:
    channel4_state.freq = freq;
    break;
  default:
    return;
  }
}

static inline void channel_update_CCR(channel_t channel, uint32_t ccr)
{
  switch (channel)
  {
  case CHANNEL1:
    CHANNEL1_4_TIMER->CCR1 = ccr;
    break;
  case CHANNEL2:
    CHANNEL1_4_TIMER->CCR2 = ccr;
    break;
  case CHANNEL3:
    CHANNEL1_4_TIMER->CCR3 = ccr;
    break;
  case CHANNEL4:
    CHANNEL1_4_TIMER->CCR4 = ccr;
    break;
  default:
    return;
  }
}

static inline void channel_update(volatile channel_state_t *channel)
{
  if (channel->enabled == 0) // Don't calculate if the channel is disabled
    return;

  if (channel->on_off == 0)
  {
    channel_update_CCR(channel->channel, (CHANNEL1_4_TIMER_ARR >> 0x1)); // Set default duty cycle to 50%
    return;
  }

  channel->count += channel->freq;

  while (channel->count > SAMPLE_FREQUENCY_MASK)
    channel->count -= SAMPLE_FREQUENCY_MASK;

  // If a square wave, calculate - save on flash
  if (channel->waveform == WAVEFORM_SQUARE)
  {
    if (channel->count < (SAMPLE_FREQUENCY_MASK >> 0x01))
      channel_update_CCR(channel->channel, 0);
    else
      channel_update_CCR(channel->channel, CHANNEL1_4_TIMER_ARR);
    return;
  }

  uint16_t ccr = channel->waveform_data[channel->count] >> (((MIDI_MAX_VAL - channel->vol) >> 4));
  channel_update_CCR(channel->channel, ccr);
}

void channel1_4_update()
{
  channel_update(&channel1_state);
  channel_update(&channel2_state);
  channel_update(&channel3_state);
  channel_update(&channel4_state);
}

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

static void reset_channel(volatile channel_state_t *channel, channel_t channel_num)
{
  channel->count = 0;
  channel->enabled = 0;
  channel->freq = 0;
  channel->on_off = 0;
  channel->vol = MIDI_MAX_VAL;
  channel->waveform_data = sine_base;
  channel->channel = channel_num;
}

static void channel1_4_timer_gpio_init()
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
}

void channel1_4_timer_init()
{
  // Enable the RCC for the Timer
  RCC_TIM3_CLK_Enable();

  channel1_4_timer_gpio_init();

  reset_channel(&channel1_state, CHANNEL1);
  reset_channel(&channel2_state, CHANNEL2);
  reset_channel(&channel3_state, CHANNEL3);
  reset_channel(&channel4_state, CHANNEL4);

  CHANNEL1_4_TIMER->PSC = CHANNEL1_4_TIMER_PSC;
  CHANNEL1_4_TIMER->ARR = CHANNEL1_4_TIMER_ARR;

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

  CHANNEL1_4_TIMER->CCR1 = (CHANNEL1_4_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL1_4_TIMER->CCR2 = (CHANNEL1_4_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL1_4_TIMER->CCR3 = (CHANNEL1_4_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
  CHANNEL1_4_TIMER->CCR4 = (CHANNEL1_4_TIMER_ARR >> 0x1); // Set default duty cycle to 50%

  CHANNEL1_4_TIMER->CR1 |= TIM_CR1_CEN; // Start the Timer
}