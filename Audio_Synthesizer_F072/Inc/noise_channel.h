/**
 *******************************************************************************
 * @file    noise_channel.h
 * @brief   DAC-based Noise Channel Control Interface Header
 *******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NOISE_CHANNEL_H
#define NOISE_CHANNEL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "audio_config.h"  // SAMPLE_FREQUENCY 등 정의

/* ========================================================================== */
/*                                                                            */
/*    Noise Channel Control Functions                                         */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Initialize the noise channel state.
 */
void noise_channel_init(void);

/**
 * @brief Enable the noise channel output.
 * @note  Starts the DAC conversion on DAC_CHANNEL_1.
 */
void noise_channel_enable(void);

/**
 * @brief Disable the noise channel output.
 * @note  Stops the DAC conversion on DAC_CHANNEL_1.
 */
void noise_channel_disable(void);

/**
 * @brief Turn the noise channel on or off.
 * @param state 1 to turn on, 0 to turn off.
 */
void noise_channel_on_off(uint8_t state);

/**
 * @brief Set the noise channel frequency.
 * @param freq Frequency parameter affecting sample index increment.
 */
void noise_channel_frequency(uint16_t freq);

/**
 * @brief Set the noise channel volume.
 * @param volume Volume level (0 ~ 127).
 */
void noise_channel_volume(uint8_t volume);

/**
 * @brief Update the noise channel output.
 * @note  This function should be called periodically.
 */
void noise_channel_update(void);

/* ========================================================================== */
/*                                                                            */
/*    DAC Initialization Functions                                            */
/*                                                                            */
/* ========================================================================== */

/**
 * @brief Initialize GPIO for DAC.
 */
void init_gpio_for_dac(void);

/**
 * @brief Initialize the DAC.
 */
void init_dac(void);

#endif /* NOISE_CHANNEL_H */
