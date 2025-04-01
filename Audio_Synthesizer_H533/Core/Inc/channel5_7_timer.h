/**
 *******************************************************************************
 * @file           : channel5_7_timer.h
 * @brief          : Channel5-7 Timer Control Interface Header
 *******************************************************************************
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

 #ifndef _CHANNEL5_7_TIMER_H_
 #define _CHANNEL5_7_TIMER_H_

 /* ========================================================================== */
 /*                                                                            */
 /*    Controller Functions                                                    */
 /*                                                                            */
 /* ========================================================================== */
 
 /**
  * @brief Enable the Channel Output
  * @param channel The channel to enable (CHANNEL5, CHANNEL6, or CHANNEL7)
  * @note  Starts the timer.
  */
 void channel5_7_enable(channel_t channel);
 
 /**
  * @brief Disable the Channel Output
  * @param channel The channel to disable (CHANNEL5, CHANNEL6, or CHANNEL7)
  * @note  Halts the timer.
  */
 void channel5_7_disable(channel_t channel);
 
 /**
  * @brief Set the current output waveform
  * @param channel The channel to modify (CHANNEL5, CHANNEL6, or CHANNEL7)
  * @param wave    The waveform to synthesize (예: WAVEFORM_NOISE)
  */
 void channel5_7_set_waveform(channel_t channel, waveforms_t wave);
 
 /**
  * @brief Turn the channel note on or off (no PWM)
  * @param channel The channel to modify (CHANNEL5, CHANNEL6, or CHANNEL7)
  * @param state   Turn the channel on (1) or off (0)
  * @note  Updates when channel5_7_update() is invoked.
  */
 void channel5_7_on_off(channel_t channel, uint8_t state);
 
 /**
  * @brief Set the channel volume
  * @param channel The channel to modify (CHANNEL5, CHANNEL6, or CHANNEL7)
  * @param volume  Volume of the signal (up to 127)
  * @note  Updates when channel5_7_update() is invoked.
  */
 void channel5_7_volume(channel_t channel, uint8_t volume);
 
 /**
  * @brief Set the channel frequency
  * @param channel The channel to modify (CHANNEL5, CHANNEL6, or CHANNEL7)
  * @param freq    Frequency of the signal to synthesize
  * @note  Updates when channel5_7_update() is invoked.
  */
 void channel5_7_frequency(channel_t channel, uint16_t freq);
 
 /**
  * @brief Update the current channel output according to its state
  * @note  Updates when channel5_7_update() is invoked.
  */
 void channel5_7_update();
 
 /* ========================================================================== */
 /*                                                                            */
 /*    Initialization Functions                                                */
 /*                                                                            */
 /* ========================================================================== */
 
 /**
  * @brief Initialize the channel5-7 timer component.
  */
 void channel5_7_timer_init();
 
 #endif /* _CHANNEL5_7_TIMER_H_ */
 
