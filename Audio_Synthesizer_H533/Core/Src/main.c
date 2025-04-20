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
#include "channel1_4_timer.h"

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
extern volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state;

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief This is the timer handler for the sample timer (we need a better description of this tbh...)
 * @param None
 * @retval None.
 */
void sample_timer_handler(uint16_t counter)
{
  // Operate on the counter variable to remove a compiler warning
  // This doesn't do anything...
  counter++;
  counter--;

  // Updated channels 1-4
  channel1_4_update();
}

/**
 * @brief This function demonstrates the synthesizer's third checkpoint.
 * @param None
 * @retval None.
 */
void checkpoint_3(void);

/**
 * @brief This function tests the USER_UART peripheral to see if it is working as intended.
 * @param None
 * @retval None.
 */
void test_USER_UART();

/**
 * @brief This function tests the MIDI_UART peripheral to see if it is working as intended.
 * @param None
 * @retval None.
 */
void test_MIDI_UART();

/* Private user code ---------------------------------------------------------*/

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

  // Test the MIDI_UART peripheral
  // test_MIDI_UART();

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