/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include <stm32f0xx_hal.h>

/* Private includes ----------------------------------------------------------*/

#include "sample_timer.h"
#include "channel_common.h"
#include "channel1_timer.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern volatile channel_state_t channel1_state;

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void sample_timer_handler(uint16_t counter)
{
  channel1_update();
}

/* ========================================================================== */
/*                                                                            */
/*        Main Loop                                                           */
/*                                                                            */
/* ========================================================================== */

int main(void)
{
  SystemClock_Config(); //Configure the system clock

  // ==== SAMPLE TIMER ====
  sample_timer_register_cb(sample_timer_handler); // Register the Sample Timer Callback
  sample_timer_init();
  
  // ==== OUTPUT CHANNELS ====
  // Channel 1
  channel1_timer_init();

  // Channel 1 Settings
  channel1_enable();
  channel1_set_waveform(WAVEFORM_SINE);
  channel1_on_off(1);
  channel1_frequency(20);
  channel1_volume(127);

  sample_timer_start();

  while(1)
  {
    HAL_Delay(50);
    channel1_state.freq += 20;
    channel1_state.freq = channel1_state.freq % 10000;
  };
}
