#ifndef _MIDI_H_
#define _MIDI_H_

#include <stdint.h>
#include "sample_timer.h"
#include "channel_common.h"

void setup_midi(void);
void set_midi(char data[]);
void midi_channle_voice_init(void);

#endif