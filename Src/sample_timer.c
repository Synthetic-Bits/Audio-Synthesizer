/**
 ******************************************************************************
 * @file    sample_timer.c
 * @brief   Sample Timer Control Interface
 * @author  Adrian Sucahyo
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 Sythetic Bits.
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
#include "sample_timer.h"
#include "audio_config.h"

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32f0xx_hal.h"

/* Function Prototypes -------------------------------------------------------*/

void sample_timer_stop();
void sample_timer_start();

void __sample_timer_handler(uint16_t counter);

uint8_t sample_timer_register_cb(sample_timer_cb_t cb);
void sample_timer_setup();
void sample_timer_init();

/* ========================================================================== */
/*                                                                            */
/*    Local Variables Definitions                                             */
/*                                                                            */
/* ========================================================================== */

#define SAMPLE_TIMER        TIM2
#define SAMPLE_TIMER_IRQ    TIM2_IRQn // Ensure to update the IRQ cb if necessary

#define SAMPLE_TIMER_PSC (1)
#define SAMPLE_TIMER_ARR (2928) // 16 kHz

static sample_timer_cb_t event_cb = __sample_timer_handler;

static volatile uint16_t counter;

/* ========================================================================== */
/*                                                                            */
/*    Interrupt Functions                                                     */
/*                                                                            */
/* ========================================================================== */

void TIM2_IRQHandler()
{
    SAMPLE_TIMER->SR &= ~(0x0001); // Clear the interrupt request
    event_cb(counter);
}

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

void sample_timer_reset()
{
    counter = 0;
}

void sample_timer_stop()
{
    SAMPLE_TIMER->CR1 &= ~(0x0001); // Disable the Timer
}

void sample_timer_start()
{
    SAMPLE_TIMER->CR1 |= (0x1); // Enable the Timer
}

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Placeholder Function for the timer callback
 */
static void __sample_timer_handler(uint16_t counter)
{
  return;
}

uint8_t sample_timer_register_cb(sample_timer_cb_t cb)
{
    event_cb = cb;
}

void sample_timer_init()
{
    SAMPLE_TIMER->PSC = SAMPLE_TIMER_PSC;
    SAMPLE_TIMER->ARR = SAMPLE_TIMER_ARR;

    counter = 0;

    SAMPLE_TIMER->DIER |= (0x1); // Enable the UDE

    NVIC_EnableIRQ(SAMPLE_TIMER_IRQ);
}