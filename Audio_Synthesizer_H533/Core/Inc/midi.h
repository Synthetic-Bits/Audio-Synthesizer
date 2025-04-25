#ifndef _MIDI_H_
#define _MIDI_H_

#include <stdint.h>
#include "sample_timer.h"
#include "channel_common.h"

void setup_midi(void);
void midi_process();
void midi_channel_voice_init(void);

typedef struct
{
  channel_t channel;
  uint16_t keynumber;
  uint16_t velocity;
  uint16_t forceonkey;
  uint16_t addressofcontrol;
  uint16_t valueofcontroloutput;
  uint16_t programmeselect;
  uint16_t pressurevalue;
  uint16_t pitchbendmsb;
  uint16_t pitchbendlsb;
  uint16_t number_of_channels;

  uint16_t frequency;
  uint8_t voice;
} midi_t;

typedef enum
{
  UNKNOWN_STATE,
  NOTE_ON_EVENT_STATE,
  NOTE_OFF_EVENT_STATE,
  POLYPHONIC_KEY_PRESSURE_STATE,
  CONTROL_MODE_STATE,
  PROGRAM_CHANGE_STATE,
  CHANNEL_PRESSURE_STATE,
  PITCH_BEND_STATE,
  SYSTEM_MESSAGE_STATE
} midi_state_t;

typedef enum
{
  LOCAL_CONTROL_ON_OFF_STATE,
  ALL_NOTES_OFF_STATE,
  OMNI_MODE_OFF_STATE,
  OMNI_MODE_ON_STATE,
  MONO_MODE_ON_STATE,
  POLY_MODE_ON_STATE,
} midi_mode_state_t;

#endif