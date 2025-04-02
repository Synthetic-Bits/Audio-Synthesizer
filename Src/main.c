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

#include "uart.h"
#include "midi.h"

#include "sample_timer.h"
#include "channel_common.h"
#include "channel1_4_timer.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state;
extern volatile int globalReceiveBufferIndex;
extern volatile char globalReceiveBuffer[1024];

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void sample_timer_handler(uint16_t counter)
{
  channel1_4_update();
}

void checkpoint_1()
{
  // Create a constant that holds the size of UART packets
  const int packetSize = 2;

  // Configure the UART3 peripheral to get MIDI signals
  configureUART3(115200, UART_ENABLE_INTERRUPTS, 2);

  while (1)
  {
    if (globalReceiveBufferIndex > packetSize)
    {
      // Reset the global receive buffer's index
      globalReceiveBufferIndex = 0;

      // Read from the global receive buffer
      char data[packetSize];
      for (int i = 0; i < packetSize; i++)
        data[i] = globalReceiveBuffer[i];

      get_midi(data);
    }
  }
}

/* ========================================================================== */
/*                                                                            */
/*        Main Loop                                                           */
/*                                                                            */
/* ========================================================================== */

int main(void)
{
  // Configure the system's clock.
  SystemClock_Config();

  // Run the checkpoint 1 code
  checkpoint_1();

  // While loop in-case something goes wrong
  while (1)
  { 
    __NOP();
  }

  // // ==== SAMPLE TIMER ====
  // sample_timer_register_cb(sample_timer_handler); // Register the Sample Timer Callback
  // sample_timer_init();
  
  // // ==== OUTPUT CHANNELS ====
  // // Channels 1 - 4
  // channel1_4_timer_init();

  // // Channel 1 Settings
  // channel1_4_enable(CHANNEL1);
  // channel1_4_set_waveform(CHANNEL1, WAVEFORM_SINE);
  // channel1_4_on_off(CHANNEL1, 1);
  // channel1_4_frequency(CHANNEL1, 100);
  // channel1_4_volume(CHANNEL1, 127);

  // // Channel 2 Settings
  // channel1_4_enable(CHANNEL2);
  // channel1_4_set_waveform(CHANNEL2, WAVEFORM_TRIG);
  // channel1_4_on_off(CHANNEL2, 1);
  // channel1_4_frequency(CHANNEL2, 100);
  // channel1_4_volume(CHANNEL2, 127);

  // // Channel 3 Settings
  // channel1_4_enable(CHANNEL3);
  // channel1_4_set_waveform(CHANNEL3, WAVEFORM_RAMP);
  // channel1_4_on_off(CHANNEL3, 1);
  // channel1_4_frequency(CHANNEL3, 100);
  // channel1_4_volume(CHANNEL3, 127);

  // // Channel 4 Settings
  // channel1_4_enable(CHANNEL4);
  // channel1_4_set_waveform(CHANNEL4, WAVEFORM_SQUARE);
  // channel1_4_on_off(CHANNEL4, 1);
  // channel1_4_frequency(CHANNEL4, 100);
  // channel1_4_volume(CHANNEL4, 127);

  // // Start the sample timer (advance the sampled waveforms)
  // sample_timer_start();

  // uint16_t current_f = 100;

  // while(1)
  // {
  //   HAL_Delay(100);

  //   current_f += 100;
  //   current_f = current_f % 10000;

  //   channel1_state.freq = current_f;
  //   channel2_state.freq = current_f;
  //   channel3_state.freq = current_f;
  //   channel4_state.freq = current_f;
  // };
}