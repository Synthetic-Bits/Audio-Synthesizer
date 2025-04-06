/**
 ******************************************************************************
 * @file           : channel_common.h
 * @brief          : Common Channel Component Defintions
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/* ========================================================================== */
/*                                                                            */
/*    Channel Definitions                                                     */
/*                                                                            */
/* ========================================================================== */

 #ifndef _CHANNEL_COMMON_H_
 #define _CHANNEL_COMMON_H_

 typedef enum
 {
    WAVEFORM_SINE,
    WAVEFORM_TRIG,
    WAVEFORM_RAMP,
    WAVEFORM_SQUARE
 } waveforms_t;

 typedef enum
 {
     CHANNEL1,
     CHANNEL2,
     CHANNEL3,
     CHANNEL4,
     CHANNEL5,
     CHANNEL6
 } channel_t;

 typedef struct
 {
    uint32_t count;
    uint16_t freq;
    uint8_t vol;
    uint8_t on_off;
    uint8_t enabled;
    channel_t channel;
    waveforms_t waveform;
    const unsigned char *waveform_data;
 } channel_state_t;

 #endif /* _CHANNEL_COMMON_H_ */