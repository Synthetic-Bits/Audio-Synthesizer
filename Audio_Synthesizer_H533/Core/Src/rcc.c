/**
 ******************************************************************************
 * @file    rcc.c
 * @brief   RCC Timer Control Interface
 * @author  Adrian Sucahyo, Kenneth Gordon, Bryant Watson, Hayoung In
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 Synthetic Bits.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "rcc.h"

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32h5xx.h"

/* Function Prototypes -------------------------------------------------------*/
void RCC_GPIOA_CLK_Enable();
void RCC_GPIOB_CLK_Enable();
void RCC_GPIOC_CLK_Enable();

void RCC_TIM2_CLK_Enable();
void RCC_TIM3_CLK_Enable();

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Enable the GPIOA Clock using the RCC.
 */
void RCC_GPIOA_CLK_Enable()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
}

/**
 * @brief Enable the GPIOB Clock using the RCC.
 */
void RCC_GPIOB_CLK_Enable()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
}

/**
 * @brief Enable the GPIOC Clock using the RCC.
 */
void RCC_GPIOC_CLK_Enable()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}

/**
 * @brief Enable the RCC Clock for TIM2
 */
void RCC_TIM2_CLK_Enable()
{
    RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
}

/**
 * @brief Enable the RCC Clock for TIM3
 */
void RCC_TIM3_CLK_Enable()
{
    RCC->APB1LENR |= RCC_APB1LENR_TIM3EN;
}