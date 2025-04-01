/**
 ******************************************************************************  
 * @file    channel5_7_timer.c  
 * @brief   Channel5-7 Timer Control Interface  
 * @author  
 ******************************************************************************  
 * @attention  
 *  
 * This code is structured similarly to channel1_4_timer.c, but uses TIM4  
 * and controls channels 5, 6, and 7.  
 *  
 ******************************************************************************  
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "channel5_7_timer.h"
#include "audio_config.h"
#include "channel_common.h"
#include "config.h"
#include "rcc.h"

#include <stdio.h>
#include <string.h>
#include "stm32h5xx_hal.h"

#include "noise_base.h"

/* Function Prototypes -------------------------------------------------------*/
void channel5_7_enable(channel_t channel);
void channel5_7_disable(channel_t channel);
void channel5_7_set_waveform(channel_t channel, waveforms_t wave);
void channel5_7_on_off(channel_t channel, uint8_t state);
void channel5_7_volume(channel_t channel, uint8_t volume);
void channel5_7_frequency(channel_t channel, uint16_t freq);
static inline void channel5_7_update_CCR(channel_t channel, uint32_t ccr);
static inline void channel5_7_update(volatile channel_state_t *channel);
void channel5_7_update();
static void reset_channel(volatile channel_state_t *channel, channel_t channel_num);
static void channel5_7_timer_gpio_init();
void channel5_7_timer_init();

/* ========================================================================== */  
/*                                                                            */  
/*    Local Variable Definitions                                            */  
/*                                                                            */  
/* ========================================================================== */

#define CHANNEL5_7_TIMER_PSC (1 - 1)
#define CHANNEL5_7_TIMER_ARR ((0x1 << 10) - 1) // same ARR value;

volatile channel_state_t channel5_state, channel6_state, channel7_state;

/* ========================================================================== */  
/*                                                                            */  
/*    Control Functions                                                     */  
/*                                                                            */  
/* ========================================================================== */

void channel5_7_enable(channel_t channel)
{
  switch (channel)
  {
  case CHANNEL5:
    channel5_state.enabled = 1;
    CHANNEL5_7_TIMER->CCER |= TIM_CCER_CC1E;
    break;
  case CHANNEL6:
    channel6_state.enabled = 1;
    CHANNEL5_7_TIMER->CCER |= TIM_CCER_CC2E;
    break;
  case /*CHANNEL7*/ 2: // Here, treat CHANNEL7 as channel 2 (using TIM4's CCR3)
    channel7_state.enabled = 1;
    CHANNEL5_7_TIMER->CCER |= TIM_CCER_CC3E;
    break;
  default:
    return;
  }
}

void channel5_7_disable(channel_t channel)
{
  switch (channel)
  {
  case CHANNEL5:
    channel5_state.enabled = 0;
    CHANNEL5_7_TIMER->CCER &= ~TIM_CCER_CC1E;
    break;
  case CHANNEL6:
    channel6_state.enabled = 0;
    CHANNEL5_7_TIMER->CCER &= ~TIM_CCER_CC2E;
    break;
  case /*CHANNEL7*/ 2:
    channel7_state.enabled = 0;
    CHANNEL5_7_TIMER->CCER &= ~TIM_CCER_CC3E;
    break;
  default:
    return;
  }
}

void channel5_7_set_waveform(channel_t channel, waveforms_t wave)
{
  const unsigned short *curr_wave;

  switch (wave)
  {
  case WAVEFORM_NOISE:
    curr_wave = noise_base;
    break;
  default:
    return;
  }

  switch (channel)
  {
  case CHANNEL5:
    channel5_state.waveform = wave;
    channel5_state.waveform_data = curr_wave;
    break;
  case CHANNEL6:
    channel6_state.waveform = wave;
    channel6_state.waveform_data = curr_wave;
    break;
  case /*CHANNEL7*/ 2:
    channel7_state.waveform = wave;
    channel7_state.waveform_data = curr_wave;
    break;
  default:
    return;
  }
}

void channel5_7_on_off(channel_t channel, uint8_t state)
{
  switch (channel)
  {
  case CHANNEL5:
    if (state)
    {
      channel5_state.on_off = 1;
      channel5_state.count = 0; // Reset counter when starting a new tone
    }
    else
      channel5_state.on_off = 0;
    break;
  case CHANNEL6:
    if (state)
    {
      channel6_state.on_off = 1;
      channel6_state.count = 0;
    }
    else
      channel6_state.on_off = 0;
    break;
  case /*CHANNEL7*/ 2:
    if (state)
    {
      channel7_state.on_off = 1;
      channel7_state.count = 0;
    }
    else
      channel7_state.on_off = 0;
    break;
  default:
    return;
  }
}

void channel5_7_volume(channel_t channel, uint8_t volume)
{
  switch (channel)
  {
  case CHANNEL5:
    channel5_state.vol = volume;
    break;
  case CHANNEL6:
    channel6_state.vol = volume;
    break;
  case /*CHANNEL7*/ 2:
    channel7_state.vol = volume;
    break;
  default:
    return;
  }
}

void channel5_7_frequency(channel_t channel, uint16_t freq)
{
  switch (channel)
  {
  case CHANNEL5:
    channel5_state.freq = freq;
    break;
  case CHANNEL6:
    channel6_state.freq = freq;
    break;
  case /*CHANNEL7*/ 2:
    channel7_state.freq = freq;
    break;
  default:
    return;
  }
}

static inline void channel5_7_update_CCR(channel_t channel, uint32_t ccr)
{
  switch (channel)
  {
  case CHANNEL5:
    CHANNEL5_7_TIMER->CCR1 = ccr;
    break;
  case CHANNEL6:
    CHANNEL5_7_TIMER->CCR2 = ccr;
    break;
  case /*CHANNEL7*/ 2:
    CHANNEL5_7_TIMER->CCR3 = ccr;
    break;
  default:
    return;
  }
}

static inline void channel5_7_update(volatile channel_state_t *channel)
{
  if (channel->enabled == 0) // Do nothing if channel is disabled
    return;

  if (channel->on_off == 0)
  {
    // Set default 50% duty cycle if channel is off
    channel5_7_update_CCR(channel->channel, (CHANNEL5_7_TIMER_ARR >> 1));
    return;
  }

  channel->count += channel->freq;

  while (channel->count > SAMPLE_FREQUENCY_MASK)
    channel->count -= SAMPLE_FREQUENCY_MASK;

  if (channel->waveform == WAVEFORM_SQUARE)
  {
    if (channel->count < (SAMPLE_FREQUENCY_MASK >> 1))
      channel5_7_update_CCR(channel->channel, 0);
    else
      channel5_7_update_CCR(channel->channel, CHANNEL5_7_TIMER_ARR);
    return;
  }

  uint16_t ccr = channel->waveform_data[channel->count] >> (((MIDI_MAX_VAL - channel->vol) >> 4));
  channel5_7_update_CCR(channel->channel, ccr);
}

void channel5_7_update()
{
  channel5_7_update(&channel5_state);
  channel5_7_update(&channel6_state);
  channel5_7_update(&channel7_state);
}

/* ========================================================================== */  
/*                                                                            */  
/*    Initialization Functions                                              */  
/*                                                                            */  
/* ========================================================================== */

static void reset_channel(volatile channel_state_t *channel, channel_t channel_num)
{
  channel->count = 0;
  channel->enabled = 0;
  channel->freq = 0;
  channel->on_off = 0;
  channel->vol = MIDI_MAX_VAL;
  channel->waveform_data = noise_base; 
  channel->channel = channel_num;
}

static void channel5_7_timer_gpio_init()
{
  RCC_GPIOB_CLK_Enable();

  GPIO_InitTypeDef initChannel5_7 = {
      CHANNEL5_GPIO_PIN | CHANNEL6_GPIO_PIN | CHANNEL7_GPIO_PIN,
      GPIO_MODE_AF_PP,
      GPIO_NOPULL,
      GPIO_SPEED_FREQ_HIGH,
      GPIO_AF2_TIM3};  // Note: using GPIO_AF2_TIM3 here (may need to change if TIM4 alternate function is different)
      
  HAL_GPIO_Init(CHANNEL5_7_GPIO_PORT, &initChannel5_7);
}

void channel5_7_timer_init()
{
  // Enable TIM4 RCC clock
  RCC_TIM4_CLK_Enable();

  channel5_7_timer_gpio_init();

  // Initialize each channel (channel numbers set according to config.h)
  reset_channel(&channel5_state, CHANNEL5);
  reset_channel(&channel6_state, CHANNEL6);
  // For CHANNEL7: if not defined in channel_common.h, use an arbitrary value (here, 2)
  reset_channel(&channel7_state, /*CHANNEL7*/ 2);

  CHANNEL5_7_TIMER->PSC = CHANNEL5_7_TIMER_PSC;
  CHANNEL5_7_TIMER->ARR = CHANNEL5_7_TIMER_ARR;

  // Configure TIM4 Channel 1 (CHANNEL5)
  CHANNEL5_7_TIMER->CCMR1 &= ~TIM_CCMR1_CC1S;
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC1M) & CHANNEL5_7_TIMER->CCMR1) | (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC1CE) & CHANNEL5_7_TIMER->CCMR1) | TIM_CCMR1_OC1CE;

  // Configure TIM4 Channel 2 (CHANNEL6)
  CHANNEL5_7_TIMER->CCMR1 &= ~TIM_CCMR1_CC2S;
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC2M) & CHANNEL5_7_TIMER->CCMR1) | (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
  CHANNEL5_7_TIMER->CCMR1 = ((~TIM_CCMR1_OC2CE) & CHANNEL5_7_TIMER->CCMR1) | TIM_CCMR1_OC2CE;

  // Configure TIM4 Channel 3 (CHANNEL7)
  CHANNEL5_7_TIMER->CCMR2 &= ~TIM_CCMR2_CC3S;
  CHANNEL5_7_TIMER->CCMR2 = ((~TIM_CCMR2_OC3M) & CHANNEL5_7_TIMER->CCMR2) | (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
  CHANNEL5_7_TIMER->CCMR2 = ((~TIM_CCMR2_OC3CE) & CHANNEL5_7_TIMER->CCMR2) | TIM_CCMR2_OC3CE;

  // Set default 50% duty cycle for all channels
  CHANNEL5_7_TIMER->CCR1 = (CHANNEL5_7_TIMER_ARR >> 1);
  CHANNEL5_7_TIMER->CCR2 = (CHANNEL5_7_TIMER_ARR >> 1);
  CHANNEL5_7_TIMER->CCR3 = (CHANNEL5_7_TIMER_ARR >> 1);

  CHANNEL5_7_TIMER->CR1 |= TIM_CR1_CEN; // Start the timer
}
