/**
 ******************************************************************************
 * @file    channel1_timer.c
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
#include "channel_common.h"
#include "channel1_timer.h"
#include "audio_config.h"

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32f0xx_hal.h"

#include "sine_base.h"

/* Function Prototypes -------------------------------------------------------*/

void channel1_enable();
void channel1_disable();
void channel1_set_waveform(waveforms_t wave);
void channel1_on_off(uint8_t state);
void channel1_volume(uint8_t volume);
void channel1_frequency(uint16_t freq);

void channel1_update();

static void channel1_timer_gpio_init();
void channel1_timer_init();

/* ========================================================================== */
/*                                                                            */
/*    Local Variables Definitions                                             */
/*                                                                            */
/* ========================================================================== */

#define CHANNEL1_TIMER TIM3 // Ensure to update the RCC if necessary

#define CHANNEL1_TIMER_PSC (1 - 1)
#define CHANNEL1_TIMER_ARR ((0x1 << 7) - 1) // ~200 kHz (186 kHz)

#define CHANNEL1_GPIO_PORT GPIOC
#define CHANNEL1_GPIO_PIN  GPIO_PIN_6

static const unsigned char *waveform_data;

volatile channel_state_t channel1_state;

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

void channel1_enable()
{
  channel1_state.enabled = 1;
  CHANNEL1_TIMER->CCER |= TIM_CCER_CC1E;
}
void channel1_disable()
{
  channel1_state.enabled = 0;
  CHANNEL1_TIMER->CCER &= ~TIM_CCER_CC1E;
}

void channel1_set_waveform(waveforms_t wave)
{
  channel1_state.waveform = wave;

  switch (wave)
  {
  case WAVEFORM_SINE:
    waveform_data = sine_base;
    break;
  default:
    break;
  }
}

void channel1_on_off(uint8_t state)
{
  if (state)
  {
    channel1_state.on_off = 1;
    channel1_state.count = 0; // Reset the counter when starting a new tone
  }
  else
    channel1_state.on_off = 0;
}

void channel1_volume(uint8_t volume)
{
  channel1_state.vol = volume;
}

void channel1_frequency(uint16_t freq)
{
  channel1_state.freq = freq;
}

void channel1_update()
{
  if (channel1_state.enabled == 0) // Don't calculate if the channel is disabled
    return;

  if (channel1_state.on_off == 0)
  {
    CHANNEL1_TIMER->CCR1 = (CHANNEL1_TIMER_ARR >> 0x1); // Set default duty cycle to 50%
    return;
  }

  channel1_state.count += channel1_state.freq;

  while (channel1_state.count > SAMPLE_FREQUENCY_MASK)
    channel1_state.count -= SAMPLE_FREQUENCY;

  uint16_t arr = waveform_data[channel1_state.count] >> (((MIDI_MAX_VAL - channel1_state.vol) >> 4) + 1);
  CHANNEL1_TIMER->CCR1 = arr;
}

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

static void channel1_timer_gpio_init()
{
  HAL_RCC_GPIOA_CLK_Enable();
  HAL_RCC_GPIOB_CLK_Enable();
  HAL_RCC_GPIOC_CLK_Enable();

  GPIO_InitTypeDef initChannel1 = {
    CHANNEL1_GPIO_PIN,
    GPIO_MODE_AF_PP,
    GPIO_NOPULL,
    GPIO_SPEED_FREQ_LOW,
    GPIO_AF0_TIM3
  };
  
  HAL_GPIO_Init(CHANNEL1_GPIO_PORT, &initChannel1);
}

void channel1_timer_init()
{
  // Enable the RCC for the Timer
  HAL_RCC_TIM3_CLK_Enable();

  channel1_timer_gpio_init();

  channel1_state.count = 0;
  channel1_state.enabled = 0;
  channel1_state.freq = 0;
  channel1_state.on_off = 0;
  channel1_state.vol = MIDI_MAX_VAL;

  waveform_data = sine_base; // Default to a sine wave
  
  CHANNEL1_TIMER->PSC = CHANNEL1_TIMER_PSC;
  CHANNEL1_TIMER->ARR = CHANNEL1_TIMER_ARR;

  CHANNEL1_TIMER->CCMR1 &= ~TIM_CCMR1_CC1S;                                                                                            // Select CCS Mode 0 (output)
  CHANNEL1_TIMER->CCMR1 = CHANNEL1_TIMER->CCMR1 = ((~TIM_CCMR1_OC1M) & CHANNEL1_TIMER->CCMR1) | (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // Enable PWM Mode 1
  CHANNEL1_TIMER->CCMR1 = ((~TIM_CCMR1_OC1CE) & CHANNEL1_TIMER->CCMR1) | TIM_CCMR1_OC1CE;                                              // Enable Clear

  CHANNEL1_TIMER->CCR1 = (CHANNEL1_TIMER_ARR >> 0x1); // Set default duty cycle to 50%

  CHANNEL1_TIMER->CR1 |= TIM_CR1_CEN; // Start the Timer
}