/**
 ******************************************************************************
 * @file           : channel_timer.h
 * @brief          : Channel1 Timer Control Interface Header
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

#include "channel_common.h"

/* ========================================================================== */
/*                                                                            */
/*    Controller Definitions                                                  */
/*                                                                            */
/* ========================================================================== */

#ifndef _CHANNEL1_TIMER_H_
#define _CHANNEL1_TIMER_H_

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Enable the Channel Output
 * @note Starts the timer
 */
void channel1_enable();

/**
 * @brief Disable the Channel Output
 * @note Halts the timer
 */
void channel1_disable();

/**
 * @brief Set the current output waveform
 * @param wave The waveform to synthesize
 */
void channel1_set_waveform(waveforms_t wave);

/**
 * @brief Turn the channel note on or off (no more PWM)
 * @param state Turn the channel on or off (1 is on, 0 is off)
 * @note Updates when channeln_update() is invoked
 */
void channel1_on_off(uint8_t state);

/**
 * @brief Set the channel volume
 * @param volume Volume of the signal (up to 127)
 * @note Updates when channeln_update() is invoked
 */
void channel1_volume(uint8_t volume);

/**
 * @brief Set the channel frequency
 * @param freq Frequency of the signal to synthesize
 * @note Updates when channeln_update() is invoked
 */
void channel1_frequency(uint16_t freq);

/**
 * @brief Update the current channel output according to its state
 * @note Updates when channeln_update() is invoked
 */
void channel1_update();

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Intialize the channel1 timer component
 */
void channel1_timer_init();

#endif /* _CHANNEL1_TIMER_H_ */