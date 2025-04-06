/**
 ******************************************************************************
 * @file           : rcc.h
 * @brief          : RCC Initialization Defintions
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/* ========================================================================== */
/*                                                                            */
/*    RCC Control Definitions                                                 */
/*                                                                            */
/* ========================================================================== */

#ifndef _RCC_H_
#define _RCC_H_

// GPIO RCC Enables
void RCC_GPIOA_CLK_Enable(void);
void RCC_GPIOB_CLK_Enable(void);
void RCC_GPIOC_CLK_Enable(void);

// Timer RCC Enables
void RCC_TIM2_CLK_Enable(void);
void RCC_TIM3_CLK_Enable(void);

#endif /* _RCC_H_ */