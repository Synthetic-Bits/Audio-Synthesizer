/**
 * gpio.c
 * 
 * This file contains the implementations for the synthesizer's GPIO functions.
 * 
 * Authors:  Kenneth Gordon, Bryant Watson, Hayoung Im, and Adrian Sucahyo
 * Date:  March 24, 2025
 */

#include "gpio.h"
#include "main.h"
#include <stm32f0xx.h>

void initializeLEDs()
{
    // Enable the RCC for GPIOC.
    HAL_RCC_GPIOC_CLK_Enable();

    // Set the LED pins in standard output mode.
    GPIO_InitTypeDef initStr = {RED_LED_PIN | GREEN_LED_PIN | BLUE_LED_PIN | ORANGE_LED_PIN,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_NOPULL,
                                GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOC, &initStr);

    // Set all of the initial values of the LEDs to OFF.
    HAL_GPIO_WritePin(GPIOC, RED_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GREEN_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, BLUE_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, ORANGE_LED_PIN, GPIO_PIN_RESET);
}