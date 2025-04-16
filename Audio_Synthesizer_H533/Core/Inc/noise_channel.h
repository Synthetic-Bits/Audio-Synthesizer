// /**
//  *******************************************************************************
//  * @file    noise_channel.h
//  * @brief   DAC-based Noise Channel Control Interface Header
//  *******************************************************************************
//  */

//  #ifndef _NOISE_CHANNEL_H_
//  #define _NOISE_CHANNEL_H_

//  #include <stdlib.h>
//  #include <stdint.h>
//  #include "channel_common.h"
//  #include "audio_config.h"

//  /* Exported Functions --------------------------------------------------------*/

//  /**
//    * @brief  Enable the noise channel output via DAC.
//    * @note   Starts the DAC conversion.
//    */
//  void noise_channel_enable(void);

//  /**
//    * @brief  Disable the noise channel output.
//    * @note   Stops the DAC conversion.
//    */
//  void noise_channel_disable(void);

//  /**
//    * @brief  Set the waveform for the noise channel.
//    * @param  wave  The waveform to use (should be WAVEFORM_NOISE).
//    */
//  void noise_channel_set_waveform(waveforms_t wave);

//  /**
//    * @brief  Turn the noise channel on or off.
//    * @param  state  1 to turn on, 0 to turn off.
//    * @note   When turned on, the sample counter is reset.
//    */
//  void noise_channel_on_off(uint8_t state);

//  /**
//    * @brief  Set the noise channel volume.
//    * @param  volume  Volume level (0 ~ 127).
//    */
//  void noise_channel_volume(uint8_t volume);

//  /**
//    * @brief  Set the noise channel frequency.
//    * @param  freq  Frequency parameter affecting sample index increment.
//    */
//  void noise_channel_frequency(uint16_t freq);

//  /**
//    * @brief  Update the DAC output based on the noise channel state.
//    * @note   This function should be called periodically (e.g., via a timer interrupt or DMA trigger)
//    *         to output the next sample from the noise waveform.
//    */
//  void noise_channel_update(void);

//  /**
//    * @brief  Initialize the DAC-based noise channel.
//    */
//  void noise_channel_init(void);

//  #endif /* _NOISE_CHANNEL_H_ */
