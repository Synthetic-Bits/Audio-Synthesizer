/**
 ******************************************************************************
 * @file           : uart.h
 * @brief          : UART Interface Header
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

/* ========================================================================== */
/*                                                                            */
/*    Definitions                                                             */
/*                                                                            */
/* ========================================================================== */

#ifndef _UART_H_
#define _UART_H_

#define UART_DISABLE_INTERRUPTS 0
#define UART_ENABLE_INTERRUPTS 1

/* ========================================================================== */
/*                                                                            */
/*    Configuration Functions                                                 */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function configures the USART1 peripheral for use in the synthesizer.
 * @param baudRate The baud rate for the USART peripheral.
 * @param enableInterrupts Whether or not interrupts should be enabled for the peripheral.
 * @param interruptPriority The interrupt priority of the peripheral.  Should be set even if interrupts are disabled for this peripheral.
 */
void configureUART1(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority);

/**
 * @brief This function configures the USART2 peripheral for use in the synthesizer.
 * @param baudRate The baud rate for the USART peripheral.
 * @param enableInterrupts Whether or not interrupts should be enabled for the peripheral.
 * @param interruptPriority The interrupt priority of the peripheral.  Should be set even if interrupts are disabled for this peripheral.
 */
void configureUART2(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority);

/**
 * @brief This function configures the USART3 peripheral for use in the synthesizer.
 * @param baudRate The baud rate for the USART peripheral.
 * @param enableInterrupts Whether or not interrupts should be enabled for the peripheral.
 * @param interruptPriority The interrupt priority of the peripheral.  Should be set even if interrupts are disabled for this peripheral.
 */
void configureUART3(unsigned int baudRate, uint8_t enableInterrupts, uint8_t interruptPriority);

/* ========================================================================== */
/*                                                                            */
/*    Sending Functions                                                       */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function sends all of the contents in a user specified buffer on the USART1 peripheral.
 * @param sendBuffer The buffer containing the data that should be sent.
 * @retval None.
 */
void sendUART1(char *sendBuffer);

/**
 * @brief This function sends all of the contents in a user specified buffer on the USART2 peripheral.
 * @param sendBuffer The buffer containing the data that should be sent.
 * @retval None.
 */
void sendUART2(char *sendBuffer);

/**
 * @brief This function sends all of the contents in a user specified buffer on the USART3 peripheral.
 * @param sendBuffer The buffer containing the data that should be sent.
 * @retval None.
 */
void sendUART3(char *sendBuffer);

/* ========================================================================== */
/*                                                                            */
/*    Blocking Receiving Functions                                            */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function receives a set number of bytes on the USART1 peripheral while blocking program execution.
 * @param nBytes The number of bytes to receive before stopping program blocking.
 * @param receiveBuffer The buffer to store received information in.
 * @retval None.
 */
void receiveUART1Blocking(int nBytes, char *receiveBuffer);

/**
 * @brief This function receives a set number of bytes on the USART2 peripheral while blocking program execution.
 * @param nBytes The number of bytes to receive before stopping program blocking.
 * @param receiveBuffer The buffer to store received information in.
 * @retval None.
 */
void receiveUART2Blocking(int nBytes,  char *receiveBuffer);

/**
 * @brief This function receives a set number of bytes on the USART3 peripheral while blocking program execution.
 * @param nBytes The number of bytes to receive before stopping program blocking.
 * @param receiveBuffer The buffer to store received information in.
 * @retval None.
 */
void receiveUART3Blocking(int nBytes, char *receiveBuffer);

#endif /* _UART_H_ */