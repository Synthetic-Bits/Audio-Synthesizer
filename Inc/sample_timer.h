/**
 ******************************************************************************
 * @file           : sample_timer.h
 * @brief          : Sample Timer Control Interface Header
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/* ========================================================================== */
/*                                                                            */
/*    Controller Definitions                                                  */
/*                                                                            */
/* ========================================================================== */

#ifndef _SAMPLE_TIMER_H_
#define _SAMPLE_TIMER_H_

typedef void (*sample_timer_cb_t)(uint16_t count);

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Reset the current count in the sample timer
 */
void sample_timer_reset();

/**
 * @brief Halt the sample timer
 */
void sample_timer_stop();

/**
 * @brief Start the sample timer
 */
void sample_timer_start();

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Register the Sample Timer Update Callback
 * @param cb Pointer to the function to invoke when timer is updated
 */
void sample_timer_register_cb(sample_timer_cb_t cb);

/**
 * @brief Intialize the sample timer component
 */
void sample_timer_init();

#endif /* _SAMPLE_TIMER_H_ */