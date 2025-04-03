/**
 ******************************************************************************
 * @file           : audio_config.h
 * @brief          : Audio Control Config Header
 ******************************************************************************
 */

#ifndef _AUDIO_CONFIG_H_
#define _AUDIO_CONFIG_H_

#define SAMPLE_FREQUENCY_BITS   14
#define SAMPLE_FREQUENCY        (uint16_t)(0x1 << SAMPLE_FREQUENCY_BITS) // 16,384 Samples / Second
#define SAMPLE_FREQUENCY_MASK   (uint16_t)(SAMPLE_FREQUENCY - 1)

#define MIDI_MAX_VAL (0x7F)
#define MIDI_MIN_VAL (0x00)

#endif /* _AUDIO_CONFIG_H_ */