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
#define CHANNEL1_GPIO_PIN GPIO_PIN_6
#define CHANNEL2_GPIO_PIN GPIO_PIN_7
#define CHANNEL3_GPIO_PIN GPIO_PIN_8
#define CHANNEL4_GPIO_PIN GPIO_PIN_9

#endif /* _CONFIG_H_ */