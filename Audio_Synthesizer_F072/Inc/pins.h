/**
 ******************************************************************************
 * @file           : pins.h
 * @brief          : Global Pin Header
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

/* ========================================================================== */
/*                                                                            */
/*    Definitions                                                             */
/*                                                                            */
/* ========================================================================== */

#ifndef _PINS_H_
#define _PINS_H_

// Timer Channels 1-4
#define CHANNEL1_GPIO_PIN // TBD
#define CHANNEL2_GPIO_PIN // TBD
#define CHANNEL3_GPIO_PIN // TBD
#define CHANNEL4_GPIO_PIN // TBD

// Timer Channels 5-7
#define CHANNEL5_GPIO_PIN // TBD
#define CHANNEL6_GPIO_PIN // TBD
#define CHANNEL7_GPIO_PIN // TBD

// Timer Channel 8
#define CHANNEL8_GPIO_PIN // TBD

// LEDs
#define RED_LED_PIN GPIO_PIN_6    // PC6
#define BLUE_LED_PIN GPIO_PIN_7   // PC7
#define ORANGE_LED_PIN GPIO_PIN_8 // PC8
#define GREEN_LED_PIN GPIO_PIN_9  // PC9

// USART
#define UART1_TX_PIN GPIO_PIN_6 // PB6
#define UART1_RX_PIN GPIO_PIN_7 // PB7

#define UART2_TX_PIN GPIO_PIN_2 // PA2
#define UART2_RX_PIN GPIO_PIN_3 // PA3

#define UART3_TX_PIN GPIO_PIN_4 // PC4
#define UART3_RX_PIN GPIO_PIN_5 // PC5

#define UART4_TX_PIN GPIO_PIN_10 // PC10
#define UART4_RX_PIN GPIO_PIN_11 // PC11

#endif /* _PINS_H_ */