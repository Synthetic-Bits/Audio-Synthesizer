/**
 ******************************************************************************
 * @file           : channel_driver.h
 * @brief          : Channel Timer Control Interface Header
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

#ifndef _CHANNEL_DRIVER_H_
#define _CHANNEL_DRIVER_H_

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Enable the Channel Output
 * @param channel The channel to enable
 * @note Starts the timer
 */
void channel_enable(channel_t channel);

/**
 * @brief Disable the Channel Output
 * @param channel The channel to disable
 * @note Halts the timer
 */
void channel_disable(channel_t channel);

/**
 * @brief Set the current output waveform
 * @param channel The channel to modify
 * @param wave The waveform to synthesize
 */
void channel_set_waveform(channel_t channel, waveforms_t wave);

/**
 * @brief Set the channel volume
 * @param channel The channel to modify
 * @param volume Volume of the signal (up to 127)
 * @note Updates when channel_update() is invoked
 */
void channel_volume(channel_t channel, uint8_t volume);

/**
 * @brief Turn the channel note on or off (no more PWM)
 * @param channel The channel to modify
 * @param voice The voice to modify
 * @param state Turn the channel on or off (1 is on, 0 is off)
 * @note Updates when channel_update() is invoked
 */
void channel_voice_on_off(channel_t channel, uint8_t voice, uint8_t state);

/**
 * @brief Set the channel frequency
 * @param channel The channel to modify
 * @param voice Voice to modify
 * @param freq Frequency of the signal to synthesize
 * @note Updates when channel_update() is invoked
 */
void channel_voice_frequency(channel_t channel, uint8_t voice, uint16_t freq);

/**
 * @brief Set the channel frequency
 * @param channel The channel to modify
 * @param voice Voice to modify
 * @param freq Frequency of the signal to synthesize
 * @note Updates when channel_update() is invoked
 */
void channel_voice_modulation(channel_t channel, uint8_t voice, uint16_t modulation);

/**
 * @brief Update the current channel output according to its state
 * @note Updates when channel_update() is invoked
 */
void channel_update_all();

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Intialize the channel1 timer component
 */
void channel_timer_init();

#endif /* _CHANNEL_DRIVER_H_ */