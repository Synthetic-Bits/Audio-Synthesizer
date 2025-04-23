/**
 ******************************************************************************
 * @file           : noise_timer.h
 * @brief          : Noise‑channel Timer / DAC Control Interface Header
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

#include "channel_common.h"
#include "audio_config.h"
#include "config.h"
#include "rcc.h"

/* ========================================================================== */
/*                                                                            */
/*    Controller Definitions                                                  */
/*                                                                            */
/* ========================================================================== */

#ifndef _NOISE_TIMER_H_
#define _NOISE_TIMER_H_

/* ========================================================================== */
/*                                                                            */
/*    Control Functions                                                       */
/*                                                                            */
/* ========================================================================== */


/**
 * @brief  Start timer and enable DAC output
 */
void noise_timer_enable(void);

/**
 * @brief  Stop timer and mute DAC output
 */
void noise_timer_disable(void);

/**
 * @brief  Turn the channel on or off
 * @param  state 1 = on, 0 = off
 */
void noise_timer_on_off(uint8_t state);

/**
 * @brief  Set output volume (0 – 255)
 */
void noise_timer_volume(uint8_t volume);

/**
 * @brief  Set playback frequency in Hz
 */
void noise_timer_frequency(uint16_t freq);

/**
 * @brief  Apply any pending parameter changes
 */
void noise_timer_update(void);

/* ========================================================================== */
/*                                                                            */
/*    Initialization Functions                                                */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief  Initialize timer/DAC for the noise channel
 */
void noise_timer_init(void);

/**
 * @brief  Configure DAC1 CH2 GPIO and registers
 * @note   Call once at start‑up
 */
void dac1_ch2_init(void);

#endif /* _NOISE_TIMER_H_ */
