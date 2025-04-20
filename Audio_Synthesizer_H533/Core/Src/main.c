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

#include "config.h"
#include "main.h"
#include "uart.h"
#include <stm32h5xx_hal.h>

/* Private includes ----------------------------------------------------------*/

#include "sample_timer.h"
#include "channel_common.h"
#include "channel_driver.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

// Checkpoint 3 Defines (only use 1!)
// #define DEMO_100Hz_SWEEP
// #define DEMO_C_MAJOR
// #define DEMO_C_MAJOR_CHORDS

// Define for configuring the UART tests
#define UART_INTERRUPT_TEST

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern volatile int global_receive_buffer_index;
extern volatile char global_receive_buffer[GLOBAL_RECEIVE_BUFFER_SIZE];
extern volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state, channel5_state, channel6_state, channel7_state;

/* Private function prototypes -----------------------------------------------*/

void checkpoint_3(void);
void test_USER_UART();
void test_MIDI_UART();

void sample_timer_handler();

void init_sample_timer();
void init_channel_driver();
void start_audio_synthesis();
void stop_audio_synthesis();

/* Private user code ---------------------------------------------------------*/

/* ========================================================================== */
/*                                                                            */
/*        Demonstration Functions                                             */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function demonstrates the synthesizer's third checkpoint.
 * @param None
 * @retval None.
 */
void checkpoint_3(void)
{

  // Test the MIDI_UART peripheral
  // test_MIDI_UART();

  // Channel 1 Settings
  channel_enable(CHANNEL1);
  channel_set_waveform(CHANNEL1, WAVEFORM_SINE);
  channel_volume(CHANNEL1, 127);

  // Channel 2 Settings
  channel_enable(CHANNEL2);
  channel_set_waveform(CHANNEL2, WAVEFORM_TRIG);
  channel_volume(CHANNEL2, 127);

  // Channel 3 Settings
  channel_enable(CHANNEL3);
  channel_set_waveform(CHANNEL3, WAVEFORM_RAMP);
  channel_volume(CHANNEL3, 127);

  // Channel 4 Settings
  channel_enable(CHANNEL4);
  channel_set_waveform(CHANNEL4, WAVEFORM_SQUARE);
  channel_volume(CHANNEL4, 127);

  while (1)
  {
  }
}

/**
 * @brief This function tests the USER_UART peripheral to see if it is working as intended.
 * @param None
 * @retval None.
 */
void test_USER_UART()
{
#ifdef UART_INTERRUPT_TEST
  // Initialize the USER_UART peripheral (interrupt-mode)
  configure_USER_UART(115200, UART_ENABLE_INTERRUPTS, 2);

  // Send info using USER_UART
  send_USER_UART("Hello from the USER_UART peripheral!\n");

  // Loop until there are five bytes in the global_receive_buffer
  char buffer[7];
  while (1)
  {
    if (global_receive_buffer_index >= 5)
    {
      // Disable interrupts while we extract data from the buffer
      NVIC_DisableIRQ(USER_UART_IRQn);

      // Extract the data from the buffer
      for (int i = 0; i < 5; i++)
        buffer[i] = global_receive_buffer[i];
      buffer[5] = '\n';
      buffer[6] = '\x0';

      // Reset the buffer index, we got the data we need
      global_receive_buffer_index = 0;

      // Re-enable interrupts as we're done extracting data from the buffer
      NVIC_EnableIRQ(USER_UART_IRQn);
      break;
    }
  }

  // Test the printu() function to see if UART console output works
  printu(buffer);
#else
  // Initialize the USER_UART peripheral (blocking-mode)
  configure_USER_UART(115200, UART_DISABLE_INTERRUPTS, 2);

  // Send info using USER_UART
  send_USER_UART("Hello from the USER_UART peripheral!\n");

  // Receive info using USER_UART
  char buffer[7];
  receive_USER_UART_blocking(5, buffer);
  buffer[5] = '\n';
  buffer[6] = '\x0';

  // Test the printu() function to see if UART console output works
  printu(buffer);
#endif
}

/**
 * @brief This function tests the MIDI_UART peripheral to see if it is working as intended.
 * @param None
 * @retval None.
 */
void test_MIDI_UART()
{
#ifdef UART_INTERRUPT_TEST
  // Initialize the MIDI_UART peripheral (interrupt-mode)
  configure_MIDI_UART(115200, UART_ENABLE_INTERRUPTS, 2);

  // Send info using MIDI_UART
  send_MIDI_UART("Hello from the MIDI_UART peripheral!\n");

  // Loop until there are five bytes in the global_receive_buffer
  char buffer[7];
  while (1)
  {
    if (global_receive_buffer_index >= 5)
    {
      // Disable interrupts while we extract data from the buffer
      NVIC_DisableIRQ(MIDI_UART_IRQn);

      // Extract the data from the buffer
      for (int i = 0; i < 5; i++)
        buffer[i] = global_receive_buffer[i];
      buffer[5] = '\n';
      buffer[6] = '\x0';

      // Reset the buffer index, we got the data we need
      global_receive_buffer_index = 0;

      // Re-enable interrupts as we're done extracting data from the buffer
      NVIC_EnableIRQ(MIDI_UART_IRQn);
      break;
    }
  }

  // Echo the received data back using MIDI_UART
  send_MIDI_UART(buffer);
#else
  // Initialize the MIDI_UART peripheral (blocking-mode)
  configure_MIDI_UART(115200, UART_DISABLE_INTERRUPTS, 2);

  // Send info using MIDI_UART
  send_MIDI_UART("Hello from the MIDI_UART peripheral!\n");

  // Receive info using MIDI_UART
  char buffer[7];
  receive_MIDI_UART_blocking(5, buffer);
  buffer[5] = '\n';
  buffer[6] = '\x0';

  // Echo the received data back using MIDI_UART
  send_MIDI_UART(buffer);
#endif
}

/* ========================================================================== */
/*                                                                            */
/*        Callback Handlers                                                   */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Sample timer callback handler - invoked when sample timer asserts
 * @param None
 * @retval None.
 */
void sample_timer_handler()
{
  channel_update_all();
}

/* ========================================================================== */
/*                                                                            */
/*        Initialization and Control Functions                                */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Initialize all the necessary components for the channel driver
 */
void init_sample_timer()
{
  // Configure and start the sample timer
  sample_timer_register_cb(sample_timer_handler); // Register the Sample Timer Callback
  sample_timer_init();
}

/**
 * @brief Initialize all the necessary components for the channel driver
 * @param None
 */
void init_channel_driver()
{
  // Channels 1 - 7
  channel_timer_init();

  channel_voice_on(CHANNEL1, 0);
  channel_voice_frequency(CHANNEL1, 0, 440);
  channel_voice_modulation(CHANNEL1, 0, 8196);

  channel_voice_on(CHANNEL2, 0);
  channel_voice_frequency(CHANNEL2, 0, 100);
  channel_voice_modulation(CHANNEL2, 0, 8196);

  channel_voice_on(CHANNEL3, 0);
  channel_voice_frequency(CHANNEL3, 0, 100);
  channel_voice_modulation(CHANNEL3, 0, 8196);

  channel_voice_on(CHANNEL4, 0);
  channel_voice_frequency(CHANNEL4, 0, 100);
  channel_voice_modulation(CHANNEL4, 0, 8196);
}

/**
 * @brief Start the overall audio synthesis
 */
void start_audio_synthesis()
{
  // Start the sample timer (advance the sampled waveforms)
  sample_timer_start();
}

/**
 * @brief Stop the overall audio synthesis
 */
void stop_audio_synthesis()
{
  // Stop the sample timer (advance the sampled waveforms)
  sample_timer_stop();
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

  // == AUDIO SETUP ==
  init_sample_timer();
  init_channel_driver();

  start_audio_synthesis();

  // Loop indefinitely to prevent returning from main
  while (1)
  {
  };
}