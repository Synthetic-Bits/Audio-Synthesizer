/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Main Header
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#pragma once
#include <stdint.h>

/* ========================================================================== */
/*                                                                            */
/*    RCC Clock Functions                                                     */
/*                                                                            */
/* ========================================================================== */

// GPIO
void HAL_RCC_GPIOA_CLK_Enable(void);
void HAL_RCC_GPIOB_CLK_Enable(void);
void HAL_RCC_GPIOC_CLK_Enable(void);

// Timer RCC Enables
void HAL_RCC_TIM2_CLK_Enable(void);
void HAL_RCC_TIM3_CLK_Enable(void);

// USART
void HAL_RCC_USART1_CLK_Enable(void);
void HAL_RCC_USART2_CLK_Enable(void);
void HAL_RCC_USART3_CLK_Enable(void);

/* ========================================================================== */
/*                                                                            */
/*    Misc Configuration Functions                                            */
/*                                                                            */
/* ========================================================================== */

// System Clock Config
void SystemClock_Config(void);