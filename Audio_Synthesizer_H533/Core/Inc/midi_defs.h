/**
 ******************************************************************************
 * @file           : midi_defs.h
 * @brief          : Pin and Component Config Header
 * @authors        : Adrian Sucahyo, Kenneth Gordon, Bryant Watson,
 *                   and Hayoung Im
 ******************************************************************************
 */

#include "main.h"

#ifndef _MIDI_DEFS_H_
#define _MIDI_DEFS_H_

// status codes------------------------------------------------------------------
#define NOTE_ON_EVENT (0b1001)
#define NOTE_OFF_EVENT (0b1000)
#define POLYPHONIC_KEY_PRESSURE (0b1010)
#define CONTROL_CHANGE (0b1011)
#define PROGRAM_CHANGE (0b1100)
#define CHANNEL_PRESSURE (0b1101)
#define PITCH_BEND (0b1110)
#define SYSTEM_MESSAGE (0b1111)

// midi bit masks------------------------------------------------------------------
#define MESSAGETYPE_msk (0xf0) // non 0x7f, 0d127
#define CHANNEL_msk (0x0f)     // non 0x7f, 0d127
#define KEYNUMBER_msk (0x7f)
#define VELOCITY_msk (0x7f)
#define FORCEONKEY_msk (0x7f)
#define ADDRESS_OF_CONTROL_msk (0x7f)
#define VALUE_OF_CONTROL_OUTPUT_msk (0x7f)
#define PROGRAMME_SELECT_msk (0x7f)
#define PRESSURE_VALUE_msk (0x7f)
#define PITCH_BEND_LSB_msk (0x7f)
#define PITCH_BEND_MSB_msk (0x7f)
#define NUMBER_OF_CHANNELS_msk (0x7f)
#define MANUFACTURERS_IDENTIFICATION_msk (0x7f)
#define SYSTEM_EXCLUSIVE_MESSAGE_msk (0x7f)
#define MIDI_TIME_CODE_QUARTER_FRAME_MESSAGE_D_msk (0x70)
#define MIDI_TIME_CODE_QUARTER_FRAME_MESSAGE_T_msk (0x0f) // non 0x7f, 0d127

// midi system message bit masks----------------------------------------------------
#define BEGIN_SYSTEM_EXCLUSIVE (0b0000)
#define MIDI_TIME_CODE (0b0001)
#define SONG_POSITION_POINTER (0b0010)
#define SONG_SELECT (0b0011)
#define TUNE_REQUEST (0b0110)
#define END_SYSTEM_EXCLUSIVE (0b0111)
#define TIMING_CLOCK (0b1000)
#define START (0b1010)
#define CONTINUE (0b1011)
#define STOP (0b1100)
#define ACTIVE_SENSING (0b1110)
#define SYSTEM_RESET (0b1111)

#endif