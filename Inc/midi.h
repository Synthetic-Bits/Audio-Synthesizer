#include <stdint.h>

typedef struct {
    uint16_t channel;
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
} MIDI;

MIDI get_midi(char data[]);