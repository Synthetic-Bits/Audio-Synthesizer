#include "uart.h"
#include <stm32f0xx_hal.h>

void configureUART1(int baudRate)
{
    // Enable the RCC for GPIOB and USART1.
    HAL_RCC_GPIOB_CLK_Enable();
    HAL_RCC_USART1_CLK_Enable();

    // Set the GPIO pins for TX (PB6) and RX (PB7) into AF mode.
    GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF1_USART1};
    HAL_GPIO_Init(GPIOB, &initStr);

    // Set the baud rate of communication to the one specified by the user.
    USART1->BRR = 0x0000FFFF & (HAL_RCC_GetHCLKFreq() / baudRate);

    // Enable the receive register not empty interrupt.
    USART1->CR1 &= ~(USART_CR1_RXNEIE);
    USART1->CR1 |=   USART_CR1_RXNEIE;

    // Enable the transmitter and receiver hardware in USART1.
    USART1->CR1 &= ~(USART_CR1_TE);
    USART1->CR1 |=   USART_CR1_TE;
    USART1->CR1 &= ~(USART_CR1_RE);
    USART1->CR1 |=   USART_CR1_RE;

    // Lastly, enable the USART1 peripheral.
    USART1->CR1 &= ~(USART_CR1_UE);
    USART1->CR1 |=   USART_CR1_UE;
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

void receiveUART1(int nBytes, char *receiveBuffer)
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

void configureUART2(int baudRate)
{
    // Enable the RCC for GPIOA and USART2.
    HAL_RCC_GPIOA_CLK_Enable();
    HAL_RCC_USART2_CLK_Enable();

    // Set the GPIO pins for TX (PA14) and RX (PA15) into AF mode.
    GPIO_InitTypeDef initStr = {GPIO_PIN_14 | GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF1_USART2};
    HAL_GPIO_Init(GPIOA, &initStr);

    // Set the baud rate of communication to the one specified by the user.
    USART2->BRR = 0x0000FFFF & (HAL_RCC_GetHCLKFreq() / baudRate);

    // Enable the receive register not empty interrupt.
    USART2->CR1 &= ~(USART_CR1_RXNEIE);
    USART2->CR1 |=   USART_CR1_RXNEIE;

    // Enable the transmitter and receiver hardware in USART2.
    USART2->CR1 &= ~(USART_CR1_TE);
    USART2->CR1 |=   USART_CR1_TE;
    USART2->CR1 &= ~(USART_CR1_RE);
    USART2->CR1 |=   USART_CR1_RE;

    // Lastly, enable the USART2 peripheral.
    USART2->CR1 &= ~(USART_CR1_UE);
    USART2->CR1 |=   USART_CR1_UE;
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

void receiveUART2(int nBytes,  char *receiveBuffer)
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