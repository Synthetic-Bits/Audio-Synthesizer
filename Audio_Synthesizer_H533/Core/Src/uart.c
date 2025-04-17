/**
******************************************************************************
* @file    uart.c
* @brief   UART Interface
* @author  Kenneth Gordon
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
#include "config.h"
#include "rcc.h"
#include "uart.h"

/* Private includes ----------------------------------------------------------*/
#include <stm32h5xx_hal.h>

/* ========================================================================== */
/*                                                                            */
/*    Local Definitions                                                       */
/*                                                                            */
/* ========================================================================== */

// Define the size of the global_receive_buffer
#define GLOBAL_RECEIVE_BUFFER_SIZE 1024

/* ========================================================================== */
/*                                                                            */
/*    Global Variables                                                        */
/*                                                                            */
/* ========================================================================== */

// Global definitions for the global_receive_buffer (used in interrupts).
volatile int global_receive_buffer_index = 0;
volatile char global_receive_buffer[1024];

// Variables that keep track of if the UART peripherals are configured.
static int USER_UART_configured = 0;
static int MIDI_UART_configured = 0;

/* ========================================================================== */
/*                                                                            */
/*    IRQ Handlers                                                            */
/*                                                                            */
/* ========================================================================== */

void USART1_IRQHandler()
{
    // Add the received data to the global buffer.
    global_receive_buffer[global_receive_buffer_index] = USER_UART->RDR;

    // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
    global_receive_buffer_index++;
}

void USART3_IRQHandler()
{
    // Add the received data to the global buffer.
    global_receive_buffer[global_receive_buffer_index] = MIDI_UART->RDR;

    // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
    global_receive_buffer_index++;
}

/* ========================================================================== */
/*                                                                            */
/*    Configuration Functions                                                 */
/*                                                                            */
/* ========================================================================== */

void configure_USER_UART(unsigned int baud_rate, uint8_t enable_interrupts, uint8_t interrupt_priority)
{
    // Enable the RCC for GPIOB and USART1 (user UART's canonical name).
    RCC_GPIOB_CLK_Enable();
    RCC_USART1_CLK_Enable();

    // Set the GPIO pins for TX (PB14) and RX (PB15) into AF mode.
    GPIO_InitTypeDef init_str = {USER_UART_TX_PIN | USER_UART_RX_PIN,
                                GPIO_MODE_AF_PP,
                                GPIO_NOPULL,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_AF4_USART1};
    HAL_GPIO_Init(USER_UART_PORT, &init_str);

    // Set the baud rate of communication to the one specified by the user.
    USER_UART->BRR = (HAL_RCC_GetHCLKFreq() / baud_rate) & 0xFFFF;

    // Enable the receive register not empty interrupt (if interrupts are enabled).
    if (enable_interrupts == UART_ENABLE_INTERRUPTS)
    {
        USER_UART->CR1 &= ~(USART_CR1_RXNEIE);
        USER_UART->CR1 |=  (USART_CR1_RXNEIE);
    }

    // Enable the transmitter and receiver hardware in USER_UART.
    USER_UART->CR1 &= ~(USART_CR1_TE);
    USER_UART->CR1 |=  (USART_CR1_TE);
    USER_UART->CR1 &= ~(USART_CR1_RE);
    USER_UART->CR1 |=  (USART_CR1_RE);

    // Enable the USER_UART peripheral.
    USER_UART->CR1 &= ~(USART_CR1_UE);
    USER_UART->CR1 |=  (USART_CR1_UE);

    // Enable interrupts for USER_UART as well as set its priority (if interrupts are enabled).
    if (enable_interrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(USER_UART_IRQn);
        NVIC_SetPriority(USER_UART_IRQn, interrupt_priority);
    }

    // Lastly, indicate that this peripheral has been configured
    USER_UART_configured = 1;
}

void configure_MIDI_UART(unsigned int baud_rate, uint8_t enable_interrupts, uint8_t interrupt_priority)
{
    // Enable the RCC for GPIOB and USART3
    RCC_GPIOB_CLK_Enable();
    RCC_USART3_CLK_Enable();

    // Set the GPIO pins for TX (PB10) and RX (PB1) into AF mode.
    GPIO_InitTypeDef init_str = {MIDI_UART_TX_PIN | MIDI_UART_RX_PIN,
                                GPIO_MODE_AF_PP,
                                GPIO_NOPULL,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_AF7_USART3};
    HAL_GPIO_Init(MIDI_UART_PORT, &init_str);

    // Set the baud rate of communication to the one specified by the user.
    MIDI_UART->BRR = (HAL_RCC_GetHCLKFreq() / baud_rate) & 0xFFFF;

    // Enable the receive register not empty interrupt (if interrupts are enabled).
    if (enable_interrupts == UART_ENABLE_INTERRUPTS)
    {
        MIDI_UART->CR1 &= ~(USART_CR1_RXNEIE);
        MIDI_UART->CR1 |=  (USART_CR1_RXNEIE);
    }

    // Enable the transmitter and receiver hardware in MIDI_UART
    MIDI_UART->CR1 &= ~(USART_CR1_TE);
    MIDI_UART->CR1 |=  (USART_CR1_TE);
    MIDI_UART->CR1 &= ~(USART_CR1_RE);
    MIDI_UART->CR1 |=  (USART_CR1_RE);

    // Enable the MIDI_UART peripheral.
    MIDI_UART->CR1 &= ~(USART_CR1_UE);
    MIDI_UART->CR1 |=  (USART_CR1_UE);

    // Enable interrupts for MIDI_UART as well as set its priority (if interrupts are enabled).
    if (enable_interrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(MIDI_UART_IRQn);
        NVIC_SetPriority(MIDI_UART_IRQn, interrupt_priority);
    }

    // Lastly, indicate that this peripheral has been configured
    MIDI_UART_configured = 1;
}

/* ========================================================================== */
/*                                                                            */
/*    Sending Functions                                                       */
/*                                                                            */
/* ========================================================================== */

void send_USER_UART(char *send_buffer)
{
    int index = 0;
    char current_byte = send_buffer[index];
    while (current_byte != '\x0')
    {
        // Wait for the transmit register to be empty by polling the TXE bit in the ISR.
        while (!(USER_UART->ISR & USART_ISR_TXE))
            __NOP();

        // Write the current_byte into the TDR.
        USER_UART->TDR = current_byte;

        // Prepare the loop for the next byte.
        index++;
        current_byte = send_buffer[index];
    }
}

void send_MIDI_UART(char *send_buffer)
{
    int index = 0;
    char current_byte = send_buffer[index];
    while (current_byte != '\x0')
    {
        // Wait for the transmit register to be empty by polling the TXE bit in the ISR.
        while (!(MIDI_UART->ISR & USART_ISR_TXE))
            __NOP();

        // Write the current_byte into the TDR.
        MIDI_UART->TDR = current_byte;

        // Prepare the loop for the next byte.
        index++;
        current_byte = send_buffer[index];
    }
}

/* ========================================================================== */
/*                                                                            */
/*    Blocking Receiving Functions                                            */
/*                                                                            */
/* ========================================================================== */

void receive_USER_UART_blocking(int n_bytes, char *receive_buffer)
{
    // Loop until all of the bytes have been read.
    for (int i = 0; i < n_bytes; i++)
    {
        // Wait for the transmit register to be empty by polling the RXNE bit in the ISR.
        while(!(USER_UART->ISR & USART_ISR_RXNE))
            __NOP();

        // Save the received_byte in the receive_buffer.
        char received_byte = USER_UART->RDR;
        receive_buffer[i] = received_byte;
    }
}

void receive_MIDI_UART_blocking(int n_bytes, char *receive_buffer)
{
    // Loop until all of the bytes have been read.
    for (int i = 0; i < n_bytes; i++)
    {
        // Wait for the transmit register to be empty by polling the RXNE bit in the ISR.
        while(!(MIDI_UART->ISR & USART_ISR_RXNE))
            __NOP();

        // Save the received_byte in the receive_buffer.
        char received_byte = MIDI_UART->RDR;
        receive_buffer[i] = received_byte;
    }
}

/* ========================================================================== */
/*                                                                            */
/*    Miscellaneous Functions                                                 */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function initializes the USER_UART peripheral and acts as a drop-in replacement for print().
 * @param text The text to print using USER_UART.
 * @retval None.
 */
void printu(char *text)
{
    // Configure USART4 if needed.
    if (USER_UART_configured == 0)
        configure_USER_UART(115200, UART_DISABLE_INTERRUPTS, 2);
    
    // Send the text using USART4.
    send_USER_UART(text);
}