/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 * @authors        : Adrian Sucahyo, Kenneth Gordon, Bryant Watson, Hayoung In
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include <stm32h5xx_hal.h>

/* Private includes ----------------------------------------------------------*/

#include "sample_timer.h"
#include "channel_common.h"
#include "channel_driver.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

// #define DEMO_100Hz_SWEEP
// #define DEMO_C_MAJOR
// #define DEMO_C_MAJOR_CHORDS

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern volatile channel_state_t CHANNEL4_state, channel2_state, channel3_state, channel4_state;

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void sample_timer_handler()
{
  channel_update_all();
}

/* ========================================================================== */
/*                                                                            */
/*        Main Loop                                                           */
/*                                                                            */
/* ========================================================================== */
/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  // Configure the System Clock
  SystemClock_Config();

  // Enable the Instruction Caching
  MX_ICACHE_Init();

  // ==== SAMPLE TIMER ====
  sample_timer_register_cb(sample_timer_handler); // Register the Sample Timer Callback
  sample_timer_init();

  // ==== OUTPUT CHANNELS ====
  // Channels 1 - 4
  channel_timer_init();

  // Channel 1 Settings
  channel_enable(CHANNEL1);
  channel_set_waveform(CHANNEL1, WAVEFORM_SINE);
  channel_volume(CHANNEL1, 127);
  channel_voice_on(CHANNEL1, 0);
  channel_voice_frequency(CHANNEL1, 0, 100);
  channel_voice_modulation(CHANNEL1, 0, 0);

  // Channel 2 Settings
  channel_enable(CHANNEL2);
  channel_set_waveform(CHANNEL2, WAVEFORM_TRIG);
  channel_volume(CHANNEL2, 127);
  channel_voice_on(CHANNEL2, 0);
  channel_voice_frequency(CHANNEL2, 0, 100);
  channel_voice_modulation(CHANNEL2, 0, 0);

  // Channel 3 Settings
  channel_enable(CHANNEL3);
  channel_set_waveform(CHANNEL3, WAVEFORM_RAMP);
  channel_volume(CHANNEL3, 127);
  channel_voice_on(CHANNEL3, 0);
  channel_voice_frequency(CHANNEL3, 0, 100);
  channel_voice_modulation(CHANNEL3, 0, 0);

  // Channel 4 Settings
  channel_enable(CHANNEL4);
  channel_set_waveform(CHANNEL4, WAVEFORM_SQUARE);
  channel_volume(CHANNEL4, 127);
  channel_voice_on(CHANNEL4, 0);
  channel_voice_frequency(CHANNEL4, 0, 100);
  channel_voice_modulation(CHANNEL4, 0, 0);

  // Start the sample timer (advance the sampled waveforms)
  sample_timer_start();

  uint16_t current_f = 100;

  while (1)
  {
#ifdef DEMO_100Hz_SWEEP
    HAL_Delay(100);

    current_f += 100;
    current_f = current_f % 12000;

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;
#endif

#ifdef DEMO_C_MAJOR
    HAL_Delay(750);

    current_f = 262; // C4

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 296; // D4

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 330; // E4

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 349; // F4

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 392; // G4

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 440; // A4

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 494; // B4

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 523; // C5

    CHANNEL4_state.freq = current_f;
    channel2_state.freq = current_f;
    channel3_state.freq = current_f;
    channel4_state.freq = current_f;
#endif

#ifdef DEMO_C_MAJOR_CHORDS

    HAL_Delay(750);

    current_f = 262; // C4

    CHANNEL4_state.freq = 262;
    channel2_state.freq = 330;
    channel3_state.freq = 392;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 296; // D4

    CHANNEL4_state.freq = 296;
    channel2_state.freq = 349;
    channel3_state.freq = 440;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 330; // E4

    CHANNEL4_state.freq = 330;
    channel2_state.freq = 392;
    channel3_state.freq = 494;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 349; // F4

    CHANNEL4_state.freq = 349;
    channel2_state.freq = 440;
    channel3_state.freq = 523;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 392; // G4

    CHANNEL4_state.freq = 392;
    channel2_state.freq = 494;
    channel3_state.freq = 587;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 440; // A4

    CHANNEL4_state.freq = 440;
    channel2_state.freq = 523;
    channel3_state.freq = 659;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 494; // B4

    CHANNEL4_state.freq = 494;
    channel2_state.freq = 587;
    channel3_state.freq = 698;
    channel4_state.freq = current_f;

    HAL_Delay(750);

    current_f = 523; // C5

    CHANNEL4_state.freq = 523;
    channel2_state.freq = 659;
    channel3_state.freq = 784;
    channel4_state.freq = current_f;
#endif
  };
}