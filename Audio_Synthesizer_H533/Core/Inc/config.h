/**
 ******************************************************************************
 * @file           : config.h
 * @brief          : Pin and Component Config Header
 ******************************************************************************
 */

#include "main.h"

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* ========================================================================== */
/*                                                                            */
/*    Sample Timer Definitions                                                */
/*                                                                            */
/* ========================================================================== */

#define SAMPLE_TIMER TIM2
#define SAMPLE_TIMER_IRQ TIM2_IRQn // Ensure to update the IRQ cb if necessary

/* ========================================================================== */
/*                                                                            */
/*    Channel 1 to 4 Definitions                                              */
/*                                                                            */
/* ========================================================================== */

#define CHANNEL1_4_TIMER TIM3 // Ensure to update the RCC if necessary

#define CHANNEL1_4_GPIO_PORT GPIOC
#define CHANNEL1_GPIO_PIN GPIO_PIN_9
#define CHANNEL2_GPIO_PIN GPIO_PIN_6
#define CHANNEL3_GPIO_PIN GPIO_PIN_7
#define CHANNEL4_GPIO_PIN GPIO_PIN_8

/* ========================================================================== */
/*                                                                            */
/*    Channel 5 to 7 Definitions                                              */
/*                                                                            */
/* ========================================================================== */

#define CHANNEL5_7_TIMER TIM4 // Ensure to update the RCC if necessary

#define CHANNEL5_7_GPIO_PORT GPIOB
#define CHANNEL5_GPIO_PIN GPIO_PIN_6
#define CHANNEL6_GPIO_PIN GPIO_PIN_7
#define CHANNEL7_GPIO_PIN GPIO_PIN_8

/* ========================================================================== */
/*                                                                            */
/*    Channel 8 Definitions                                                   */
/*                                                                            */
/* ========================================================================== */

#define CHANNEL8_DAC DAC1 // Ensure to update the RCC if necessary
#define CHANNEL8_DAC_CH DAC1_CHANNEL_2

#define CHANNEL8_GPIO_PORT GPIOA
#define CHANNEL8_GPIO_PIN GPIO_PIN_5

/* ========================================================================== */
/*                                                                            */
/*    UART Definitions                                                        */
/*                                                                            */
/* ========================================================================== */

#define USER_UART UART1

#define USER_UART_PORT GPIOB
#define USER_UART_TX_PIN GPIO_PIN_14
#define USER_UART_RX_PIN GPIO_PIN_15

#define MIDI_UART UART3

#define MIDI_UART_PORT GPIOB
#define MIDI_UART_TX_PIN GPIO_PIN_10
#define MIDI_UART_RX_PIN GPIO_PIN_1

/* ========================================================================== */
/*                                                                            */
/*    DEBUG LED Definitions                                                   */
/*                                                                            */
/* ========================================================================== */

#define DEBUG_LED_PORT1 PORTC
#define DEBUG_LED_PORT2 PORTA
#define DEBUG_LED_PORT3 PORTF

#define DEBUG_LED0_PIN GPIO_PIN_10 // PC10
#define DEBUG_LED1_PIN GPIO_PIN_12 // PC11
#define DEBUG_LED2_PIN GPIO_PIN_15 // PA15
#define DEBUG_LED3_PIN GPIO_PIN_13 // PC13
#define DEBUG_LED4_PIN GPIO_PIN_14 // PC14
#define DEBUG_LED5_PIN GPIO_PIN_15 // PC15
#define DEBUG_LED6_PIN GPIO_PIN_0  // PF0
#define DEBUG_LED7_PIN GPIO_PIN_1  // PF1
#define DEBUG_LED8_PIN GPIO_PIN_2  // PC2
#define DEBUG_LED9_PIN GPIO_PIN_3  // PC3
#define DEBUG_LED10_PIN GPIO_PIN_1 // PC1
#define DEBUG_LED11_PIN GPIO_PIN_0 // PC0

#endif /* _CONFIG_H_ */