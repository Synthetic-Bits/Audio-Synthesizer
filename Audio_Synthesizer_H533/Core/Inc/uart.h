/**
******************************************************************************
* @file           : uart.h
* @brief          : UART Interface Header
******************************************************************************
*/

#include <stdint.h>

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
 * @brief This function configures the USER_UART peripheral for use in the synthesizer.
 * @param baud_rate The baud rate for the USER_UART peripheral.
 * @param enable_interrupts Whether or not interrupts should be enabled for the peripheral.
 * @param interrupt_priority The interrupt priority of the peripheral.  Should be set even if interrupts are disabled for this peripheral.
 */
void configure_USER_UART(unsigned int baud_rate, uint8_t enable_interrupts, uint8_t interrupt_priority);

/**
 * @brief This function configures the MIDI_UART peripheral for use in the synthesizer.
 * @param baud_rate The baud rate for the MIDI_UART peripheral.
 * @param enable_interrupts Whether or not interrupts should be enabled for the peripheral.
 * @param interrupt_priority The interrupt priority of the peripheral.  Should be set even if interrupts are disabled for this peripheral.
 */
void configure_MIDI_UART(unsigned int baud_rate, uint8_t enable_interrupts, uint8_t interrupt_priority);

/* ========================================================================== */
/*                                                                            */
/*    Sending Functions                                                       */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function sends all of the contents in a user specified buffer on the USER_UART peripheral.
 * @param send_buffer The buffer containing the data that should be sent.
 * @retval None.
 */
void send_USER_UART(char *send_buffer);

/**
 * @brief This function sends all of the contents in a user specified buffer on the MIDI_UART peripheral.
 * @param send_buffer The buffer containing the data that should be sent.
 * @retval None.
 */
void send_MIDI_UART(char *send_buffer);

/* ========================================================================== */
/*                                                                            */
/*    Blocking Receiving Functions                                            */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function receives a set number of bytes on the USER_UART peripheral while blocking program execution.
 * @param n_bytes The number of bytes to receive before stopping program blocking.
 * @param receive_buffer The buffer to store received information in.
 * @retval None.
 */
void receive_USER_UART_blocking(int n_bytes, char *receive_buffer);

/**
 * @brief This function receives a set number of bytes on the MIDI_UART peripheral while blocking program execution.
 * @param n_bytes The number of bytes to receive before stopping program blocking.
 * @param receive_buffer The buffer to store received information in.
 * @retval None.
 */
void receive_MIDI_UART_blocking(int n_bytes,  char *receive_buffer);

/* ========================================================================== */
/*                                                                            */
/*    Miscellaneous Functions                                                 */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief This function initializes the USER_UART peripheral and acts as a drop-in replacement for print().
 * @param text The text to print using the USER_UART peripheral.
 * @retval None.
 */
void printu(char *text);

#endif /* _UART_H_ */