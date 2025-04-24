#ifndef _MIDI_H_
#define _MIDI_H_

#include <stdint.h>
#include "sample_timer.h"
#include "channel_common.h"

void setup_midi(void);
void set_midi(uint8_t data[]);
void midi_channel_voice_init(void);

#endif