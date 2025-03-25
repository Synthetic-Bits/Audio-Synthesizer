/**
 * gpio.h
 * 
 * This file contains the public defines and definitions for the synthesizer's GPIO functions.
 * 
 * Authors:  Kenneth Gordon, Bryant Watson, Hayoung Im, and Adrian Sucahyo
 * Date:  March 24, 2025
 */

#ifndef GPIO_H
#define GPIO_H

// Defines to more easily identify the LED pins.
#define RED_LED_PIN GPIO_PIN_6
#define BLUE_LED_PIN GPIO_PIN_7
#define ORANGE_LED_PIN GPIO_PIN_8
#define GREEN_LED_PIN GPIO_PIN_9

/**
 * @brief Function that initializes the LEDs on the microcontroller.
 * @param None.
 * @retval None.
 */
void initializeLEDs();

#endif // GPIO_H