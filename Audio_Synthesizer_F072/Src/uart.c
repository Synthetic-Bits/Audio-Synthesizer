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
#include "gpio.h"
#include "main.h"
#include "uart.h"

/* Private includes ----------------------------------------------------------*/
#include <stm32f0xx_hal.h>

/* ========================================================================== */
/*                                                                            */
/*    Pin Definitions                                                         */
/*                                                                            */
/* ========================================================================== */

// Defines for the TX pins of UART1, UART2, UART3, and UART4
#define UART1_TX_PIN GPIO_PIN_6
#define UART2_TX_PIN GPIO_PIN_2
#define UART3_TX_PIN GPIO_PIN_4
#define UART4_TX_PIN GPIO_PIN_10

// Defines for the RX pins of UART1, UART2, UART3, and UART4
#define UART1_RX_PIN GPIO_PIN_7
#define UART2_RX_PIN GPIO_PIN_3
#define UART3_RX_PIN GPIO_PIN_5
#define UART4_RX_PIN GPIO_PIN_11

// Define the size of the globalReceiveBuffer
#define GLOBAL_RECEIVE_BUFFER_SIZE 1024

/* ========================================================================== */
/*                                                                            */
/*    Global Variables                                                        */
/*                                                                            */
/* ========================================================================== */

// Global definitions for the globalReceiveBuffer (used in interrupts).
volatile int globalReceiveBufferIndex = 0;
volatile char globalReceiveBuffer[1024];

// Variables that keep track of if the USART peripherals are configured.
static int USART1_configured = 0;
static int USART2_configured = 0;
static int USART3_configured = 0;
static int USART4_configured = 0;

/* ========================================================================== */
/*                                                                            */
/*    IRQ Handlers                                                            */
/*                                                                            */
/* ========================================================================== */

void USART1_IRQHandler()
{
    // Add the received data to the global buffer.
    globalReceiveBuffer[globalReceiveBufferIndex] = USART1->RDR;

    // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
    globalReceiveBufferIndex++;

    // Check that the buffer hasn't overflown
    // assert(globalReceiveBufferIndex < GLOBAL_RECEIVE_BUFFER_SIZE);
}

void USART2_IRQHandler()
{
    // Add the received data to the global buffer.
    globalReceiveBuffer[globalReceiveBufferIndex] = USART2->RDR;

    // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
    globalReceiveBufferIndex++;

    // Check that the buffer hasn't overflown
    // assert(globalReceiveBufferIndex < GLOBAL_RECEIVE_BUFFER_SIZE);
}

void USART3_4_IRQHandler()
{
    // Check if USART3 or USART4 triggered the interrupt.
    if ((USART3->ISR & USART_ISR_RXNE_Msk))
    {
        // Add the received data to the global buffer.
        globalReceiveBuffer[globalReceiveBufferIndex] = USART3->RDR;

        // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
        globalReceiveBufferIndex++;
        if (globalReceiveBufferIndex > GLOBAL_RECEIVE_BUFFER_SIZE)
        {
            // We overflowed, set up the LED indicators
            initializeLEDs();
            HAL_GPIO_WritePin(GPIOC, GREEN_LED_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, ORANGE_LED_PIN, GPIO_PIN_SET);

            // Loop infinitely
            while (1) { }
        }
    }
    else if ((USART4->ISR & USART_ISR_RXNE_Msk))
    {
        // Add the received data to the global buffer.
        globalReceiveBuffer[globalReceiveBufferIndex] = USART4->RDR;

        // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
        globalReceiveBufferIndex++;
        if (globalReceiveBufferIndex > GLOBAL_RECEIVE_BUFFER_SIZE)
        {
            // We overflowed, set up the LED indicators
            initializeLEDs();
            HAL_GPIO_WritePin(GPIOC, GREEN_LED_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, ORANGE_LED_PIN, GPIO_PIN_SET);

            // Loop infinitely
            while (1) { }
        }
    }
    else
    {
        // Error state, how did you even get here?
        // Set up the LED indicators for the error
        initializeLEDs();
        HAL_GPIO_WritePin(GPIOC, RED_LED_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GREEN_LED_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, BLUE_LED_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, ORANGE_LED_PIN, GPIO_PIN_SET);

        // Loop infinitely
        while(1) { }
    }
}

/* ========================================================================== */
/*                                                                            */
/*    Configuration Functions                                                 */
/*                                                                            */
/* ========================================================================== */

void configureUART1(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority)
{
    // Enable the RCC for GPIOB and USART1.
    HAL_RCC_GPIOB_CLK_Enable();
    HAL_RCC_USART1_CLK_Enable();

    // Set the GPIO pins for TX (PB6) and RX (PB7) into AF mode.
    GPIO_InitTypeDef initStr = {UART1_TX_PIN | UART1_RX_PIN,
                                GPIO_MODE_AF_PP,
                                GPIO_NOPULL,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_AF0_USART1};
    HAL_GPIO_Init(GPIOB, &initStr);

    // Set the baud rate of communication to the one specified by the user.
    USART1->BRR = (HAL_RCC_GetHCLKFreq() / baudRate) & 0x0000FFFF;

    // Enable the receive register not empty interrupt (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        USART1->CR1 &= ~(USART_CR1_RXNEIE);
        USART1->CR1 |=   USART_CR1_RXNEIE;
    }

    // Enable the transmitter and receiver hardware in USART1.
    USART1->CR1 &= ~(USART_CR1_TE);
    USART1->CR1 |=   USART_CR1_TE;
    USART1->CR1 &= ~(USART_CR1_RE);
    USART1->CR1 |=   USART_CR1_RE;

    // Enable the USART1 peripheral.
    USART1->CR1 &= ~(USART_CR1_UE);
    USART1->CR1 |=   USART_CR1_UE;

    // Enable the USART1 interrupt in the NVIC as well as set its priority (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, interruptPriority);
    }

    // Lastly, indicate that this peripheral has been configured
    USART1_configured = 1;
}

void configureUART2(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority)
{
    // Enable the RCC for GPIOA and USART2.
    HAL_RCC_GPIOA_CLK_Enable();
    HAL_RCC_USART2_CLK_Enable();

    // Set the GPIO pins for TX (PA2) and RX (PA3) into AF mode.
    GPIO_InitTypeDef initStr = {UART2_TX_PIN | UART2_RX_PIN,
                                GPIO_MODE_AF_PP,
                                GPIO_NOPULL,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_AF1_USART2};
    HAL_GPIO_Init(GPIOA, &initStr);

    // Set the baud rate of communication to the one specified by the user.
    USART2->BRR = (HAL_RCC_GetHCLKFreq() / baudRate) & 0x0000FFFF;

    // Enable the receive register not empty interrupt (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        USART2->CR1 &= ~(USART_CR1_RXNEIE);
        USART2->CR1 |=   USART_CR1_RXNEIE;
    }

    // Enable the transmitter and receiver hardware in USART2.
    USART2->CR1 &= ~(USART_CR1_TE);
    USART2->CR1 |=   USART_CR1_TE;
    USART2->CR1 &= ~(USART_CR1_RE);
    USART2->CR1 |=   USART_CR1_RE;

    // Enable the USART2 peripheral.
    USART2->CR1 &= ~(USART_CR1_UE);
    USART2->CR1 |=   USART_CR1_UE;

    // Enable the USART2 interrupt in the NVIC as well as set its priority (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, interruptPriority);
    }

    // Lastly, indicate that this peripheral has been configured
    USART2_configured = 1;
}

void configureUART3(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority)
{
    // Enable the RCC for GPIOC and USART3.
    HAL_RCC_GPIOC_CLK_Enable();
    HAL_RCC_USART3_CLK_Enable();

    // Set the GPIO pins for TX (PC4) and RX (PC5) into AF mode.
    GPIO_InitTypeDef initStr = {UART3_TX_PIN | UART3_RX_PIN,
                                GPIO_MODE_AF_PP,
                                GPIO_NOPULL,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_AF1_USART3};
    HAL_GPIO_Init(GPIOC, &initStr);

    // Set the baud rate of communication to the one specified by the user.
    USART3->BRR = (HAL_RCC_GetHCLKFreq() / baudRate) & 0x0000FFFF;

    // Enable the receive register not empty interrupt (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        USART3->CR1 &= ~(USART_CR1_RXNEIE);
        USART3->CR1 |=   USART_CR1_RXNEIE;
    }

    // Enable the transmitter and receiver hardware in USART3.
    USART3->CR1 &= ~(USART_CR1_TE);
    USART3->CR1 |=   USART_CR1_TE;
    USART3->CR1 &= ~(USART_CR1_RE);
    USART3->CR1 |=   USART_CR1_RE;

    // Enable the USART3 peripheral.
    USART3->CR1 &= ~(USART_CR1_UE);
    USART3->CR1 |=   USART_CR1_UE;

    // Enable the USART3 interrupt in the NVIC as well as set its priority (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(USART3_4_IRQn);
        NVIC_SetPriority(USART3_4_IRQn, interruptPriority);
    }

    // Lastly, indicate that this peripheral has been configured
    USART3_configured = 1;
}

void configureUART4(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority)
{
    // Enable the RCC for GPIOC and USART4.
    HAL_RCC_GPIOC_CLK_Enable();
    HAL_RCC_USART4_CLK_Enable();

    // Set the GPIO pins for TX (PC10) and RX (PC11) into AF mode.
    GPIO_InitTypeDef initStr = {UART4_TX_PIN | UART4_RX_PIN,
                                GPIO_MODE_AF_PP,
                                GPIO_NOPULL,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_AF4_USART4};
    HAL_GPIO_Init(GPIOC, &initStr);

    // Set the baud rate of communication to the one specified by the user.
    USART4->BRR = (HAL_RCC_GetHCLKFreq() / baudRate) & 0x0000FFFF;

    // Enable the receive register not empty interrupt (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        USART4->CR1 &= ~(USART_CR1_RXNEIE);
        USART4->CR1 |=   USART_CR1_RXNEIE;
    }

    // Enable the transmitter and receiver hardware in USART4.
    USART4->CR1 &= ~(USART_CR1_TE);
    USART4->CR1 |=   USART_CR1_TE;
    USART4->CR1 &= ~(USART_CR1_RE);
    USART4->CR1 |=   USART_CR1_RE;

    // Enable the USART4 peripheral.
    USART4->CR1 &= ~(USART_CR1_UE);
    USART4->CR1 |=   USART_CR1_UE;

    // Enable the USART4 interrupt in the NVIC as well as set its priority (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(USART3_4_IRQn);
        NVIC_SetPriority(USART3_4_IRQn, interruptPriority);
    }

    // Lastly, indicate that this peripheral has been configured
    USART4_configured = 1;
}

/* ========================================================================== */
/*                                                                            */
/*    Sending Functions                                                       */
/*                                                                            */
/* ========================================================================== */

void sendUART1(char *sendBuffer)
{
    int index = 0;
    char currentByte = sendBuffer[index];
    while (currentByte != '\x0')
    {
        // Wait for the transmit register to be empty by polling the TXE bit in the ISR.
        while (!(USART1->ISR & USART_ISR_TXE))
            __NOP();

        // Write the characterToSend into the TDR.
        USART1->TDR = currentByte;

        // Prepare the loop for the next byte.
        index++;
        currentByte = sendBuffer[index];
    }
}

void sendUART2(char *sendBuffer)
{
    int index = 0;
    char currentByte = sendBuffer[index];
    while (currentByte != '\x0')
    {
        // Wait for the transmit register to be empty by polling the TXE bit in the ISR.
        while (!(USART2->ISR & USART_ISR_TXE))
            __NOP();

        // Write the characterToSend into the TDR.
        USART2->TDR = currentByte;

        // Prepare the loop for the next byte.
        index++;
        currentByte = sendBuffer[index];
    }
}

void sendUART3(char *sendBuffer)
{
    int index = 0;
    char currentByte = sendBuffer[index];
    while (currentByte != '\x0')
    {
        // Wait for the transmit register to be empty by polling the TXE bit in the ISR.
        while (!(USART3->ISR & USART_ISR_TXE))
            __NOP();

        // Write the characterToSend into the TDR.
        USART3->TDR = currentByte;

        // Prepare the loop for the next byte.
        index++;
        currentByte = sendBuffer[index];
    }    
}

void sendUART4(char *sendBuffer)
{
    int index = 0;
    char currentByte = sendBuffer[index];
    while (currentByte != '\x0')
    {
        // Wait for the transmit register to be empty by polling the TXE bit in the ISR.
        while (!(USART4->ISR & USART_ISR_TXE))
            __NOP();

        // Write the characterToSend into the TDR.
        USART4->TDR = currentByte;

        // Prepare the loop for the next byte.
        index++;
        currentByte = sendBuffer[index];
    }    
}

/* ========================================================================== */
/*                                                                            */
/*    Blocking Receiving Functions                                            */
/*                                                                            */
/* ========================================================================== */

void receiveUART1Blocking(int nBytes, char *receiveBuffer)
{
    // Loop until all of the bytes have been read.
    for (int i = 0; i < nBytes; i++)
    {
        // Wait for the transmit register to be empty by polling the RXNE bit in the ISR.
        while(!(USART1->ISR & USART_ISR_RXNE))
            __NOP();

        // Save the receivedByte in the receiveBuffer.
        char receivedByte = USART1->RDR;
        receiveBuffer[i] = receivedByte;
    }
}

void receiveUART2Blocking(int nBytes,  char *receiveBuffer)
{
    // Loop until all of the bytes have been read.
    for (int i = 0; i < nBytes; i++)
    {
        // Wait for the transmit register to be empty by polling the RXNE bit in the ISR.
        while(!(USART2->ISR & USART_ISR_RXNE))
            __NOP();

        // Save the receivedByte in the receiveBuffer.
        char receivedByte = USART2->RDR;
        receiveBuffer[i] = receivedByte;
    }
}

void receiveUART3Blocking(int nBytes, char *receiveBuffer)
{
    // Loop until all of the bytes have been read.
    for (int i = 0; i < nBytes; i++)
    {
        // Wait for the transmit register to be empty by polling the RXNE bit in the ISR.
        while(!(USART3->ISR & USART_ISR_RXNE))
            __NOP();

        // Save the receivedByte in the receiveBuffer.
        char receivedByte = USART3->RDR;
        receiveBuffer[i] = receivedByte;
    }    
}

void receiveUART4Blocking(int nBytes, char *receiveBuffer)
{
    // Loop until all of the bytes have been read.
    for (int i = 0; i < nBytes; i++)
    {
        // Wait for the transmit register to be empty by polling the RXNE bit in the ISR.
        while(!(USART4->ISR & USART_ISR_RXNE))
            __NOP();

        // Save the receivedByte in the receiveBuffer.
        char receivedByte = USART4->RDR;
        receiveBuffer[i] = receivedByte;
    }    
}

/* ========================================================================== */
/*                                                                            */
/*    Miscellaneous Functions                                                 */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function initializes the UART4 peripheral and acts as a drop-in replacement for print().
 * @param text The text to print using UART4.
 * @retval None.
 */
void printu(char *text)
{
    // Configure USART4 if needed.
    if (USART4_configured == 0)
        configureUART4(115200, UART_DISABLE_INTERRUPTS, 2);
    
    // Send the text using USART4.
    sendUART4(text);
}