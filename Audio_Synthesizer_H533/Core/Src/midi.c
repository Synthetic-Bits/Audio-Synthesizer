/**
******************************************************************************
* @file    midi.c
* @brief   MIDI Interface
* @author  Bryant Watson, Adrian Sucahyo
******************************************************************************
* @attention
*
* Copyright (c) 2025 Synthetic Bits.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

#include "uart.h"
#include "midi.h"
#include "midi_defs.h"
#include "notes.h"

#include "config.h"
#include "sample_timer.h"
#include "channel_common.h"
#include "channel_driver.h"

/* ========================================================================== */
/*                                                                            */
/*    Global Variables                                                        */
/*                                                                            */
/* ========================================================================== */

extern volatile channel_state_t channel1_state, channel2_state, channel3_state, channel4_state, channel5_state, channel6_state, channel7_state, channel8_state;

extern volatile uint16_t midi_receive_head, midi_receive_tail;
extern volatile uint8_t midi_receive_buffer[MIDI_RECEIVE_BUFFER_SIZE];

static volatile midi_t current_midi;
static volatile midi_state_t midi_state;
static volatile uint8_t midi_step;

static inline uint8_t get_statuscode(uint8_t data)
{
  return ((data & MESSAGETYPE_msk) >> 4);
}

// helper functions--------------------------------------------------------------

static void channel_mode_messages_handler(uint8_t data)
{
  // if((data[index + 1] == 0b01111010) && (data[index + 2] == 0b00000000))
  // {
  //     //printf("Local Control  Off\n");
  //     //Local Control  Off
  // }
  // if((data[index + 1] == 0b01111010) && (data[index + 2] == 0b01111111))
  // {
  //     //printf("Local Control  On\n");
  //     //Local Control  On
  // }
  // if((data[index + 1] == 0b01111011) && (data[index + 2] == 0b00000000))
  // {
  //     //printf("All Notes Off\n");
  //     //All Notes Off
  // }
  // if((data[index + 1] == 0b01111100) && (data[index + 2] == 0b00000000))
  // {
  //     //printf("Omni Mode Off\n");
  //     //Omni Mode Off
  // }
  // if((data[index + 1] == 0b01111101) && (data[index + 2] == 0b00000000))
  // {
  //     //printf("Omni Mode ON\n");
  //     //Omni Mode ON
  // }
  // if(data[index + 1] == 0b01111110)
  // {
  //     number_of_channels = (data[index + 2] & NUMBER_OF_CHANNELS_msk);
  //     //printf("Mono mode On\n\tnumber of channels:0x%02X\n", number_of_channels);
  //     //Mono mode On
  // }
}

static void system_message_handler(uint8_t data)
{
  switch (data & 0x0F)
  {
  case BEGIN_SYSTEM_EXCLUSIVE:
    // printf("BEGIN_SYSTEM_EXCLUSIVE\n");
    break;
  case MIDI_TIME_CODE:
    // printf("midi_t_TIME_CODE\n");
    break;
  case SONG_POSITION_POINTER:
    // printf("SONG_POSITION_POINTER\n");
    break;
  case SONG_SELECT:
    // printf("SONG_SELECT\n");
    break;
  case TUNE_REQUEST:
    // printf("TUNE_REQUEST\n");
    break;
  case END_SYSTEM_EXCLUSIVE:
    // printf("END_SYSTEM_EXCLUSIVE\n");
    break;
  case TIMING_CLOCK:
    // printf("TIMING_CLOCK\n");
    break;
  case START:
    // printf("START\n");
    break;
  case CONTINUE:
    // printf("CONTINUE\n");
    break;
  case STOP:
    // printf("STOP\n");
    break;
  case ACTIVE_SENSING:
    // printf("ACTIVE_SENSING\n");
    break;
  case SYSTEM_RESET:
    // printf("SYSTEM_RESET\n");
    break;
  }
}

// midi_t Set functions--------------------------------------------------------------
static inline void set_channel(uint8_t data)
{
  uint8_t midi_channel = (data & CHANNEL_msk);

  if (midi_channel > 8)
    current_midi.channel = CHANNEL8; // default channel if channel is out of range
  else
    current_midi.channel = midi_channel;
}

static inline void set_keynumber(uint8_t data)
{
  current_midi.keynumber = (data & KEYNUMBER_msk);
}

static inline void set_velocity(uint8_t data)
{
  current_midi.velocity = (data & VELOCITY_msk);
}

static inline void set_forceonkey(uint8_t data)
{
  current_midi.forceonkey = (data & FORCEONKEY_msk);
}

static inline void set_addressofcontrol(uint8_t data)
{
  current_midi.addressofcontrol = (data & ADDRESS_OF_CONTROL_msk);
}

static inline void set_valueofcontroloutput(uint8_t data)
{
  current_midi.valueofcontroloutput = (data & VALUE_OF_CONTROL_OUTPUT_msk);
}

static inline void set_programmeselect(uint8_t data)
{
  current_midi.programmeselect = (data & PROGRAMME_SELECT_msk);
}

static inline void set_pressurevalue(uint8_t data)
{
  current_midi.pressurevalue = (data & PRESSURE_VALUE_msk);
}

static inline void set_pitchbendlsb(uint8_t data)
{
  current_midi.pitchbendlsb = (data & PITCH_BEND_LSB_msk);
}

static inline void set_pitchbendmsb(uint8_t data)
{
  current_midi.pitchbendmsb = (data & PITCH_BEND_MSB_msk);
}

// note we start at C2 65hz if less then return 60hz
uint16_t midi_note_get_frequency(uint16_t key_num)
{
  key_num -= 36; // shift to C2 from C0 (12 notes * 3 octives = 36)
  if (key_num < 0)
    return FREQ_DEFAULT;

  // funciton globals
  uint16_t octave_num = 0;

  // make shift modulo x %= 12 to get notes A-G and octive
  while (key_num >= 12)
  {
    key_num -= 12;
    octave_num++;
  }

  // Starting at Octave 2
  switch (key_num)
  {
  case 0:
    return (FREQ_C2 << octave_num);
  case 1:
    return (FREQ_Db2 << octave_num);
  case 2:
    return (FREQ_D2 << octave_num);
  case 3:
    return (FREQ_Eb2 << octave_num);
  case 4:
    return (FREQ_E2 << octave_num);
  case 5:
    return (FREQ_F2 << octave_num);
  case 6:
    return (FREQ_Gb2 << octave_num);
  case 7:
    return (FREQ_G2 << octave_num);
  case 8:
    return (FREQ_Ab2 << octave_num);
  case 9:
    return (FREQ_A2 << octave_num);
  case 10:
    return (FREQ_Bb2 << octave_num);
  case 11:
    return (FREQ_B2 << octave_num);
  default:
    return FREQ_DEFAULT;
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// midi_t VOICES
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define MAX_NUM_VOICES 8
#define VOICE_MAX_FREQ 0xFFFF

uint16_t voice_channels[8][MAX_NUM_VOICES]; // double array for channel index vs map
uint8_t voice_channel_map[8] = {1, 1, 1, 1, 1, 1, 1, 1};

void midi_channel_voice_init()
{
  for (int j = 0; j < 8; j++)
  {
    for (int i = 0; i < MAX_NUM_VOICES; i++)
    {
      voice_channels[j][i] = VOICE_MAX_FREQ;
    }
  }

  voice_channel_map[0] = channel1_state.num_voices;
  voice_channel_map[1] = channel2_state.num_voices;
  voice_channel_map[2] = channel3_state.num_voices;
  voice_channel_map[3] = channel4_state.num_voices;
  voice_channel_map[4] = channel5_state.num_voices;
  voice_channel_map[5] = channel6_state.num_voices;
  voice_channel_map[6] = channel7_state.num_voices;
  voice_channel_map[7] = channel8_state.num_voices;
}

static inline uint8_t find_max_freq_voice(uint16_t voices[], uint8_t num_voices)
{
  uint8_t index = 0x00;                                // init index
  uint16_t max = 0x0000;                               // init max
  for (uint8_t voice = 0; voice < num_voices; voice++) // loop through voices
  {
    if ((voices[voice] > max)) // condition if new max is found
    {
      max = voices[voice]; // set max freq found
      index = voice;       // set max freq index
    }
  }
  return index; // return max freq index
}

static inline void voice_add_freq(uint16_t freq, uint16_t voices[], uint8_t num_voices, channel_t channel)
{
  uint8_t max_freq_voice = find_max_freq_voice(voices, num_voices); // find max freq index

  // If there are no empty voices, return
  if (voices[max_freq_voice] != VOICE_MAX_FREQ)
    return;

  voices[max_freq_voice] = freq;                          // remove the highest freq
  channel_voice_frequency(channel, max_freq_voice, freq); // set voice freq
  channel_voice_on(channel, max_freq_voice);              // turn on voice
}

static inline void voice_remove_freq(uint16_t freq, uint16_t voices[], uint8_t num_voices, channel_t channel)
{
  for (uint8_t voice = 0; voice < num_voices; voice++) // loop through voices
  {
    if (voices[voice] == freq) // if frequency is found
    {
      voices[voice] = VOICE_MAX_FREQ;    // set found frequency to VOICE_MAX_FREQ for future use
      channel_voice_off(channel, voice); // turn off voice
      return;
    }
  }
}

static uint8_t get_voice_num_from_freq(uint16_t freq, uint16_t voices[], uint8_t num_voices, channel_t channel)
{
  for (uint8_t voice = 0; voice < num_voices; voice++)
    if (voices[voice] == freq)
      return voice;

  return 0xFF;
}

/* ========================================================================== */
/*                                                                            */
/*    MIDI Processsor Functions                                               */
/*                                                                            */
/* ========================================================================== */
static inline void process_state()
{
  uint8_t data = midi_receive_buffer[midi_receive_tail];

  switch (midi_state)
  {
  case SYSTEM_MESSAGE_STATE:
    if (midi_step == 0)
    {
      system_message_handler(data);
      midi_state = UNKNOWN_STATE;
    }
    break;
  case NOTE_ON_EVENT_STATE:
    if (midi_step == 0)
    {
      set_channel(data);
      midi_step++;
    }
    else if (midi_step == 1)
    {
      set_keynumber(data);
      midi_step++;
    }
    else if (midi_step == 2)
    {
      set_velocity(data);
      current_midi.frequency = midi_note_get_frequency(current_midi.keynumber);
      voice_add_freq(current_midi.frequency, voice_channels[current_midi.channel], voice_channel_map[current_midi.channel], current_midi.channel);
      midi_state = UNKNOWN_STATE;
    }
    break;
  case NOTE_OFF_EVENT_STATE:
    if (midi_step == 0)
    {
      set_channel(data);
      midi_step++;
    }
    else if (midi_step == 1)
    {
      set_keynumber(data);
      midi_step++;
    }
    else if (midi_step == 2)
    {
      set_velocity(data);
      current_midi.frequency = midi_note_get_frequency(current_midi.keynumber);
      voice_remove_freq(current_midi.frequency, voice_channels[current_midi.channel], voice_channel_map[current_midi.channel], current_midi.channel);
      midi_state = UNKNOWN_STATE;
    }
    break;
  case POLYPHONIC_KEY_PRESSURE_STATE:
    if (midi_step == 0)
    {
      set_channel(data);
      midi_step++;
    }
    else if (midi_step == 1)
    {
      set_keynumber(data);
      midi_step++;
    }
    else if (midi_step == 2)
    {
      set_forceonkey(data);
      current_midi.frequency = midi_note_get_frequency(current_midi.keynumber);
      current_midi.voice = get_voice_num_from_freq(current_midi.frequency, voice_channels[current_midi.channel], voice_channel_map[current_midi.channel], current_midi.channel);
      channel_voice_velocity(current_midi.channel, current_midi.voice, current_midi.forceonkey);
      midi_state = UNKNOWN_STATE;
    }
    break;
  case CONTROL_CHANGE_STATE:
    if (midi_step == 0)
    {
      set_channel(data);
      midi_step++;
    }
    else if (midi_step == 1)
    {
      channel_mode_messages_handler(data);
      midi_step++;
    }
    else if (midi_step == 3)
    {
      set_addressofcontrol(data);
      midi_step++;
    }
    else if (midi_step == 4)
    {
      set_valueofcontroloutput(data);
      midi_state = UNKNOWN_STATE;
    }
    break;
  case PROGRAM_CHANGE_STATE:
    if (midi_step == 0)
    {
      set_channel(data);
      midi_step++;
    }
    else if (midi_step == 1)
    {
      set_programmeselect(data);
      midi_state = UNKNOWN_STATE;
    }
    break;
  case CHANNEL_PRESSURE_STATE:
    if (midi_step == 0)
    {
      set_channel(data);
      midi_step++;
    }
    else if (midi_step == 1)
    {
      set_pressurevalue(data);
      midi_state = UNKNOWN_STATE;
    }
    break;
  case PITCH_BEND_STATE:
    if (midi_step == 0)
    {
      set_channel(data);
      midi_step++;
    }
    else if (midi_step == 1)
    {
      set_pitchbendlsb(data);
      midi_step++;
    }
    else if (midi_step == 2)
    {
      set_pitchbendmsb(data);
      // channel_voice_modulation(current_midi.channel, ((uint16_t)current_midi.pitchbendmsb << 7) | current_midi.pitchbendlsb));
      midi_state = UNKNOWN_STATE;
    }
    break;
  default:
    midi_state = UNKNOWN_STATE;
    midi_step = 0;
    break;
  }
}

static void update_state()
{
  midi_step = 0;

  switch (get_statuscode(midi_receive_tail))
  {
  case SYSTEM_MESSAGE:
    midi_state = SYSTEM_MESSAGE_STATE;
    break;
  case NOTE_ON_EVENT:
    midi_state = NOTE_OFF_EVENT_STATE;
    break;
  case NOTE_OFF_EVENT:
    midi_state = NOTE_OFF_EVENT_STATE;
    break;
  case POLYPHONIC_KEY_PRESSURE:
    midi_state = POLYPHONIC_KEY_PRESSURE_STATE;
    break;
  case CONTROL_CHANGE:
    midi_state = CONTROL_CHANGE_STATE;
    break;
  case PROGRAM_CHANGE:
    midi_state = PROGRAM_CHANGE_STATE;
    break;
  case CHANNEL_PRESSURE:
    midi_state = CHANNEL_PRESSURE_STATE;
    break;
  case PITCH_BEND:
    midi_state = PITCH_BEND_STATE;
    break;
  default:
    midi_state = UNKNOWN_STATE;
    break;
  }
}

void midi_process()
{
  while (midi_receive_tail != midi_receive_head)
  {
    if (midi_state == UNKNOWN_STATE) // Try and find the current state
    {
      update_state();
    }

    process_state();

    midi_receive_tail++;
    midi_receive_tail &= MIDI_RECEIVE_BUFFER_MASK;
  }
}

// === KEEP IN CASE ===
// while (data[index] != 0x00)
// {
//   switch (get_statuscode(data))
//   {
//   case SYSTEM_MESSAGE:
//     system_message_handler(data);
//     // TODO
//     break;
//   case NOTE_ON_EVENT:
//     set_channel(data, &midi);
//     set_keynumber(data, &midi);
//     set_velocity(data, &midi);
//     freq = midi_note_get_frequency(midi.keynumber);
//     channel_num = midi.channel;
//     voice_add_freq(freq, voice_channels[channel_num], voice_channel_map[channel_num], channel_num); // TODO fix num of voices
//     // channel1_4_on_off(midi.channel,ON);
//     // channel1_4_frequency(midi.channel, midi_note_get_frequency(midi.keynumber));
//     // channel1_4_volume(CHANNEL1,midi.velocity);
//     // printf("NOTE_ON_EVENT: \n\tchannel:0x%02X\n\tKey Number:0x%02X\n\tvelocity:0x%02X\n\n", channel, keynumber, velocity);
//     break;
//   case NOTE_OFF_EVENT:
//     set_channel(data, &midi);
//     set_keynumber(data, &midi);
//     set_velocity(data, &midi);
//     freq = midi_note_get_frequency(midi.keynumber);
//     channel_num = midi.channel;
//     voice_remove_freq(freq, voice_channels[channel_num], voice_channel_map[channel_num], channel_num);
//     // channel1_4_on_off(midi.channel, OFF);
//     // channel1_4_frequency(midi.channel, midi_note_get_frequency(midi.keynumber));
//     // channel1_4_volume(CHANNEL1, midi.velocity);
//     // printf("NOTE_OFF_EVENT: \n\tchannel:0x%02X\n\tKey Number:0x%02X\n\tvelocity:0x%02X\n\n", channel, keynumber, velocity);
//     break;
//   case POLYPHONIC_KEY_PRESSURE:
//     set_channel(data, &midi);
//     set_keynumber(data, &midi);
//     set_forceonkey(data, &midi);
//     freq = midi_note_get_frequency(midi.keynumber);
//     channel_num = midi.channel;
//     uint8_t voice_num = get_voice_num_from_freq(freq, voice_channels[channel_num], voice_channel_map[channel_num], channel_num); // untested function
//     channel_voice_velocity(channel_num, voice_num, midi.forceonkey);
//     // printf("POLYPHONIC_KEY_PRESSURE: \n\tchannel:0x%02X\n\tKey Number:0x%02X\n\tforceonkey:0x%02X\n\n", channel, keynumber,forceonkey);
//     break;
//   case CONTROL_CHANGE:
//     set_channel(data, &midi);
//     channel_mode_messages_handler(data, midi); //
//     set_addressofcontrol(data, &midi);
//     set_valueofcontroloutput(data, &midi);
//     // printf("CONTROL_CHANGE: \n\tchannel:0x%02X\n\taddress of control:0x%02X\n\tvalue of controloutput:0x%02X\n\n", channel, addressofcontrol, forceonkey);
//   case PROGRAM_CHANGE:
//     set_channel(data, &midi);
//     set_programmeselect(data, &midi);
//     // printf("PROGRAM_CHANGE \n\tchannel:0x%02X\n\taddress of control:0x%02X\n\n", channel,programmeselect);
//     break;
//   case CHANNEL_PRESSURE:
//     set_channel(data, &midi);
//     set_pressurevalue(data, &midi);
//     // printf("PROGRAM_CHANGE: \n\tchannel:0x%02X\n\tpressurevalue:0x%02X\n\n", channel, pressurevalue);
//     break;
//   case PITCH_BEND:
//     set_channel(data, &midi);
//     set_pitchbendlsb(data, &midi);
//     set_pitchbendmsb(data, &midi);
//     uint16_t pitchbend = ((uint16_t)midi.pitchbendmsb << 7) | midi.pitchbendlsb;
//     channel_num = midi.channel;
//     // channel_voice_modulation(channel_num, pitchbend);
//     // printf("PITCH_BEND: \n\tchannel:0x%02X\n\tpitch bend lsb_msk:0x%02X\n\tpitch bend msb_msk:0x%02X\n\n", channel, pitchbendlsb, pitchbendmsb);
//     break;
//   default:
//     index++;
//     break;
//   }
