/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 * @authors        : Adrian Sucahyo, Kenneth Gordon, Bryant Watson,
 *                   and Hayoung Im
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
#include "channel1_4_timer.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

// Checkpoint 3 Defines (only use 1!)
// #define DEMO_100Hz_SWEEP
#define DEMO_C_MAJOR
// #define DEMO_C_MAJOR_CHORDS

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state;

/* Private function prototypes -----------------------------------------------*/

void checkpoint_3(void);

/* Private user code ---------------------------------------------------------*/

void sample_timer_handler(uint16_t counter)
{
  channel1_4_update();
}

void checkpoint_3(void)
{
  // Configure the output channels (1-4)
  channel1_4_timer_init();

  // Channel 1 Settings
  channel1_4_enable(CHANNEL1);
  channel1_4_set_waveform(CHANNEL1, WAVEFORM_SINE);
  channel1_4_on_off(CHANNEL1, 1);
  channel1_4_frequency(CHANNEL1, 100);
  channel1_4_volume(CHANNEL1, 127);

  // Channel 2 Settings
  channel1_4_enable(CHANNEL2);
  channel1_4_set_waveform(CHANNEL2, WAVEFORM_TRIG);
  channel1_4_on_off(CHANNEL2, 1);
  channel1_4_frequency(CHANNEL2, 100);
  channel1_4_volume(CHANNEL2, 127);

  // Channel 3 Settings
  channel1_4_enable(CHANNEL3);
  channel1_4_set_waveform(CHANNEL3, WAVEFORM_RAMP);
  channel1_4_on_off(CHANNEL3, 1);
  channel1_4_frequency(CHANNEL3, 100);
  channel1_4_volume(CHANNEL3, 127);

  // Channel 4 Settings
  channel1_4_enable(CHANNEL4);
  channel1_4_set_waveform(CHANNEL4, WAVEFORM_SQUARE);
  channel1_4_on_off(CHANNEL4, 1);
  channel1_4_frequency(CHANNEL4, 100);
  channel1_4_volume(CHANNEL4, 127);

  // Configure and start the sample timer
  sample_timer_register_cb(sample_timer_handler); // Register the Sample Timer Callback
  sample_timer_init();
  sample_timer_start(); // Advance the sampled waveforms

  // Loop through the demo code
  uint16_t current_f = 100;
  while (1)
  {
    #if defined(DEMO_100Hz_SWEEP)
        HAL_Delay(100);

        current_f += 100;
        current_f = current_f % 12000;

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;
    #elif defined(DEMO_C_MAJOR)
        HAL_Delay(750);

        current_f = 262; // C4

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 296; // D4

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 330; // E4

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 349; // F4

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 392; // G4

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 440; // A4

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 494; // B4

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 523; // C5

        channel1_state.freq = current_f;
        channel2_state.freq = current_f;
        channel3_state.freq = current_f;
        channel4_state.freq = current_f;
    #elif defined(DEMO_C_MAJOR_CHORDS)

        HAL_Delay(750);

        current_f = 262; // C4

        channel1_state.freq = 262;
        channel2_state.freq = 330;
        channel3_state.freq = 392;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 296; // D4

        channel1_state.freq = 296;
        channel2_state.freq = 349;
        channel3_state.freq = 440;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 330; // E4

        channel1_state.freq = 330;
        channel2_state.freq = 392;
        channel3_state.freq = 494;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 349; // F4

        channel1_state.freq = 349;
        channel2_state.freq = 440;
        channel3_state.freq = 523;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 392; // G4

        channel1_state.freq = 392;
        channel2_state.freq = 494;
        channel3_state.freq = 587;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 440; // A4

        channel1_state.freq = 440;
        channel2_state.freq = 523;
        channel3_state.freq = 659;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 494; // B4

        channel1_state.freq = 494;
        channel2_state.freq = 587;
        channel3_state.freq = 698;
        channel4_state.freq = current_f;

        HAL_Delay(750);

        current_f = 523; // C5

        channel1_state.freq = 523;
        channel2_state.freq = 659;
        channel3_state.freq = 784;
        channel4_state.freq = current_f;
    #endif
  }
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

  // Run the third checkpoint
  checkpoint_3();

  // Loop indefinitely to prevent returning from main
  while (1) { };
}