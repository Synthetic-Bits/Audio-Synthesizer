/**
 ******************************************************************************
 * @file           : gpio.h
 * @brief          : GPIO Interface Header
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

/* ========================================================================== */
/*                                                                            */
/*    Definitions                                                             */
/*                                                                            */
/* ========================================================================== */

#ifndef _GPIO_H_
#define _GPIO_H_

#define RED_LED_PIN GPIO_PIN_6
#define BLUE_LED_PIN GPIO_PIN_7
#define ORANGE_LED_PIN GPIO_PIN_8
#define GREEN_LED_PIN GPIO_PIN_9

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Function that initializes the LEDs on the microcontroller.
 * @param None.
 * @retval None.
 */
void initializeLEDs();

#endif /* _GPIO_H_ */