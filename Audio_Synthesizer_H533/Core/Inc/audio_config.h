/**
 ******************************************************************************
 * @file           : audio_config.h
 * @brief          : Audio Control Config Header
 * @authors        : Adrian Sucahyo, Kenneth Gordon, Bryant Watson,
 *                   and Hayoung Im
 ******************************************************************************
 */

#ifndef _AUDIO_CONFIG_H_
#define _AUDIO_CONFIG_H_

#define SAMPLE_FREQUENCY_BITS 15
#define SAMPLE_FREQUENCY (uint16_t)(0x1 << SAMPLE_FREQUENCY_BITS) // 65535 Samples / Second
#define SAMPLE_FREQUENCY_MASK (uint16_t)(SAMPLE_FREQUENCY - 1)

#define SAMPLE_MODULATION_BITS 18
#define SAMPLE_MODULATION (uint32_t)(0x1 << SAMPLE_MODULATION_BITS)
#define SAMPLE_MODULATION_MASK (uint32_t)(SAMPLE_MODULATION - 1)

#define SAMPLE_VELOCITY_BITS 14
#define SAMPLE_VELOCITY (uint16_t)(0x1 << SAMPLE_VELOCITY_BITS)
#define SAMPLE_VELOCITY_MASK (uint16_t)(SAMPLE_VELOCITY - 1)

#define SAMPLE_RESOLUTION_BITS 10
#define SAMPLE_RESOLUTION (uint16_t)(0x1 << SAMPLE_RESOLUTION_BITS) // 1024 bits
#define SAMPLE_RESOLUTION_MASK (uint16_t)(SAMPLE_RESOLUTION - 1)

#define MIDI_MAX_VAL (0x7F)
#define MIDI_MIN_VAL (0x00)

#endif /* _AUDIO_CONFIG_H_ */