#pragma once
#include <stdint.h>

// GPIO RCC Enables
void HAL_RCC_GPIOA_CLK_Enable(void);
void HAL_RCC_GPIOB_CLK_Enable(void);
void HAL_RCC_GPIOC_CLK_Enable(void);

// Timer RCC Enables
void HAL_RCC_TIM2_CLK_Enable(void);
void HAL_RCC_TIM3_CLK_Enable(void);

// System Clock Config
void SystemClock_Config(void);

