#include "assert.h"
#include "gpio.h"
#include "main.h"
#include "uart.h"
#include <stm32f0xx_hal.h>

// Defines for the TX and RX pins of UART1 and UART2
#define UART1_TX_PIN GPIO_PIN_6
#define UART2_TX_PIN GPIO_PIN_2
#define UART1_RX_PIN GPIO_PIN_7
#define UART2_RX_PIN GPIO_PIN_3

// Define the size of the globalReceiveBuffer
#define GLOBAL_RECEIVE_BUFFER_SIZE 1024

// Global definitions for the globalReceiveBuffer (used in interrupts).
int globalReceiveBufferIndex = 0;
char globalReceiveBuffer[1024];

void USART1_IRQHandler()
{
    // Add the received data to the global buffer.
    globalReceiveBuffer[globalReceiveBufferIndex] = USART1->RDR;

    // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
    globalReceiveBufferIndex++;

    // Check that the buffer hasn't overflown
    assert(globalReceiveBufferIndex < GLOBAL_RECEIVE_BUFFER_SIZE);
}

void USART2_IRQHandler()
{
    // Add the received data to the global buffer.
    globalReceiveBuffer[globalReceiveBufferIndex] = USART2->RDR;

    // Increment the buffer index by 1 and check that we haven't overflowed the buffer!
    globalReceiveBufferIndex++;

    // Check that the buffer hasn't overflown
    assert(globalReceiveBufferIndex < GLOBAL_RECEIVE_BUFFER_SIZE);
}

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
    USART1->BRR = 0x0000FFFF & (HAL_RCC_GetHCLKFreq() / baudRate);

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

    // Lastly, enable the USART1 interrupt in the NVIC as well as set its priority (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, interruptPriority);
    }
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
    USART2->BRR = 0x0000FFFF & (HAL_RCC_GetHCLKFreq() / baudRate);

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

    // Lastly, enable the USART2 interrupt in the NVIC as well as set its priority (if interrupts are enabled).
    if (enableInterrupts == UART_ENABLE_INTERRUPTS)
    {
        NVIC_EnableIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, interruptPriority);
    }
}

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

void testUART()
{
    // Initialize the LEDs for debugging.
    initializeLEDs();

    // Configure the UART peripherals.
    // configureUART1(115200, UART_ENABLE_INTERRUPTS, 2);
    configureUART2(115200, UART_ENABLE_INTERRUPTS, 2);

    char *test = "HIIIII!";
    sendUART2(test);
    receiveUART2Blocking(5, test);

    // // Send data from UART1 to UART2 and see if it was successfully sent and received.
    // char *testDataUART1 = "Hello from UART1!\n\r";
    // sendUART1(testDataUART1);

    // char test[5];
    // receiveUART1Blocking(5, test);

    /*

    int index = 0;
    char currentCharacter = testDataUART1[index];
    while(currentCharacter != '\x0')
    {
        // Check if the values match!
        if (testDataUART1[index] != globalReceiveBuffer[index])
        {
            HAL_GPIO_WritePin(GPIOC, RED_LED_PIN, GPIO_PIN_SET);
            while (1)
            {
                __NOP();
            }
        }

        // Continue looping otherwise.
        index++;
        currentCharacter = testDataUART1[index];
    }

    // Reset the globalReceiveBufferIndex for the next test.
    globalReceiveBufferIndex = 0;

    // Send data from UART2 to UART1 and see if it was successfully sent and received.
    char *testDataUART2 = "Hello from UART2!\n\r";
    sendUART2(testDataUART2);

    index = 0;
    currentCharacter = testDataUART2[index];
    while(currentCharacter != '\x0')
    {
        // Check if the values match!
        if (testDataUART1[index] != globalReceiveBuffer[index])
        {
            HAL_GPIO_WritePin(GPIOC, BLUE_LED_PIN, GPIO_PIN_SET);
            while (1)
            {
                __NOP();
            }
        }

        // Continue looping otherwise.
        index++;
        currentCharacter = testDataUART2[index];
    }

    // Indicate to the user that the test is done running.
    HAL_GPIO_WritePin(GPIOC, GREEN_LED_PIN, GPIO_PIN_SET);
    */
}