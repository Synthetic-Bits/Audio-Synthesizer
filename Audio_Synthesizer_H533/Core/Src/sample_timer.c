/**
 ******************************************************************************
 * @file    sample_timer.c
 * @brief   Sample Timer Control Interface
 * @authors Adrian Sucahyo, Kenneth Gordon, Bryant Watson, Hayoung In
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

#include "sample_timer.h"

#include "audio_config.h"
#include "config.h"
#include "rcc.h"

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32h5xx_hal.h"

/* Function Prototypes -------------------------------------------------------*/

static void __sample_timer_handler(uint16_t counter);

void sample_timer_reset();
void sample_timer_stop();
void sample_timer_start();
void sample_timer_register_cb(sample_timer_cb_t cb);
void sample_timer_init();

/* ========================================================================== */
/*                                                                            */
/*    Local Variables Definitions                                             */
/*                                                                            */
/* ========================================================================== */

#define SAMPLE_TIMER_PSC (0)
#define SAMPLE_TIMER_ARR (3813) // 65536 kHz

static sample_timer_cb_t event_cb = __sample_timer_handler;
static volatile uint16_t counter;

/* ========================================================================== */
/*                                                                            */
/*    Interrupt Functions                                                     */
/*                                                                            */
/* ========================================================================== */

void TIM2_IRQHandler()
{
  // Clear the interrupt request
  SAMPLE_TIMER->SR &= ~(0x0001);

  event_cb(counter);
  counter++;
}

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

void sample_timer_reset()
{
  // Reset the timer's counter
  counter = 0;
}

void sample_timer_stop()
{
  // Disable the timer
  SAMPLE_TIMER->CR1 &= ~(0x1);
}

void sample_timer_start()
{
  // Enable the timer
  SAMPLE_TIMER->CR1 |= (0x1);
}

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

void sample_timer_register_cb(sample_timer_cb_t cb)
{
  event_cb = cb;
}

void sample_timer_init()
{
  // Enable the RCC for the Timer
  RCC_TIM2_CLK_Enable();

  // Initialize the timer
  SAMPLE_TIMER->PSC = SAMPLE_TIMER_PSC;
  SAMPLE_TIMER->ARR = SAMPLE_TIMER_ARR;

  counter = 0;

  SAMPLE_TIMER->DIER |= (0x1); // Enable the UDE

  NVIC_EnableIRQ(SAMPLE_TIMER_IRQ);
}

/* ========================================================================== */
/*                                                                            */
/*    Placeholder Callback Functions                                          */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Placeholder Function for the timer callback
 */
static void __sample_timer_handler(uint16_t counter)
{
  return;
}