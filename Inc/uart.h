/**
 * uart.h
 * 
 * This file contains the public defines and definitions for the synthesizer's UART functions.
 * 
 * Authors:  Kenneth Gordon, Bryant Watson, Hayoung Im, and Adrian Sucahyo
 * Date:  March 25, 2025
 */

#ifndef UART_H
#define UART_H

// Defines that control whether or not interrupts are enabled.
#define UART_DISABLE_INTERRUPTS 0
#define UART_ENABLE_INTERRUPTS 1

// Configuration Functions
void configureUART1(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority);
void configureUART2(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority);
void configureUART3(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority);

// Sending Functions
void sendUART1(char *sendBuffer);
void sendUART2(char *sendBuffer);
void sendUART3(char *sendBuffer);

// Blocking Receiving Functions
void receiveUART1Blocking(int nBytes, char *receiveBuffer);
void receiveUART2Blocking(int nBytes,  char *receiveBuffer);
void receiveUART3Blocking(int nBytes, char *receiveBuffer);

// Testing Functions
void testUART(void);

#endif // UART_H