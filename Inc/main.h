#pragma once
#include <stdint.h>

// -=-= HAL RCC Clock Functions =-=-

// GPIO
void HAL_RCC_GPIOA_CLK_Enable(void);
void HAL_RCC_GPIOB_CLK_Enable(void);
void HAL_RCC_GPIOC_CLK_Enable(void);

// USART
void HAL_RCC_USART1_CLK_Enable(void);
void HAL_RCC_USART2_CLK_Enable(void);

// -=-= Misc Configuration Functions =-=-

void SystemClock_Config(void);