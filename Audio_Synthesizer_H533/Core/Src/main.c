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
#include "uart.h"
#include <stm32h5xx_hal.h>

/* Private includes ----------------------------------------------------------*/

#include "sample_timer.h"
#include "channel_common.h"
#include "channel1_4_timer.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

// #define UART_INTERRUPT_TEST

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern volatile int global_receive_buffer_index;
extern volatile char global_receive_buffer[1024];
extern volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state;

/* Private function prototypes -----------------------------------------------*/

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

void test_USER_UART()
{
  #ifdef UART_INTERRUPT_TEST
    // Initialize the USER_UART peripheral (interrupt-mode)
    configure_USER_UART(115200, UART_ENABLE_INTERRUPTS, 2);

    // Send info using USER_UART
    send_USER_UART("Hello from the USER_UART peripheral!\n");

    // Loop until there are five bytes in the global_receive_buffer
    char buffer[6];
    while(global_receive_buffer_index >= 5)
    {
      for (int i = 0; i < 5; i++)
        buffer[i] = global_receive_buffer[i];
      buffer[5] = '\n';
    }

    // Test the printu() function to see if UART console output works
    printu(buffer);
  #else
    // Initialize the USER_UART peripheral (blocking-mode)
    configure_USER_UART(115200, UART_DISABLE_INTERRUPTS, 2);

    // Send info using USER_UART
    send_USER_UART("Hello from the USER_UART peripheral!\n");
  
    // Receive info using USER_UART
    char buffer[6];
    receive_USER_UART_blocking(5, buffer);
    buffer[5] = '\n';
  
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
    char buffer[6];
    while(global_receive_buffer_index >= 5)
    {
      for (int i = 0; i < 5; i++)
        buffer[i] = global_receive_buffer[i];
      buffer[5] = '\n';
    }

    // Test the printu() function to see if UART console output works
    printu(buffer);
  #else
    // Initialize the MIDI_UART peripheral (blocking-mode)
    configure_MIDI_UART(115200, UART_DISABLE_INTERRUPTS, 2);

    // Send info using MIDI_UART
    send_MIDI_UART("Hello from the MIDI_UART peripheral!\n");

    // Receive info using MIDI_UART
    char buffer[6];
    receive_MIDI_UART_blocking(5, buffer);
    buffer[5] = '\n';

    // Test the printu() function to see if UART console output works
    printu(buffer);
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

  // Test the USER_UART peripheral
  test_USER_UART();

  // Test the MIDI_UART peripheral
  // test_MIDI_UART();

  // Loop indefinitely
  while (1) { };
}