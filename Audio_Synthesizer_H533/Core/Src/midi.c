#include <stdint.h>
#include <stdio.h>

#include "sample_timer.h"
#include "channel_common.h"
#include "channel1_4_timer.h"
#include "main.h"
#include <stm32h5xx_hal.h>
#include "midi.h"

//#include <stm32f0xx_hal.h> ??
// #include "uart.h" ??
// #include "midi.h" ?? 
// #include "gpio.h" ??



//status codes------------------------------------------------------------------
#define NOTE_ON_EVENT           (0b1001)
#define NOTE_OFF_EVENT          (0b1000)
#define POLYPHONIC_KEY_PRESSURE (0b1010)
#define CONTROL_CHANGE          (0b1011)
#define PROGRAM_CHANGE          (0b1100)
#define CHANNEL_PRESSURE        (0b1101)
#define PITCH_BEND              (0b1110)
#define SYSTEM_MESSAGE          (0b1111)

//midi bit masks------------------------------------------------------------------
#define MESSAGETYPE_msk                             (0xf0) //non 0x7f, 0d127
#define CHANNEL_msk                                 (0x0f) //non 0x7f, 0d127
#define KEYNUMBER_msk                               (0x7f)
#define VELOCITY_msk                                (0x7f)
#define FORCEONKEY_msk                              (0x7f)
#define ADDRESS_OF_CONTROL_msk                      (0x7f)
#define VALUE_OF_CONTROL_OUTPUT_msk                 (0x7f)
#define PROGRAMME_SELECT_msk                        (0x7f)
#define PRESSURE_VALUE_msk                          (0x7f)
#define PITCH_BEND_LSB_msk                          (0x7f)
#define PITCH_BEND_MSB_msk                          (0x7f)
#define NUMBER_OF_CHANNELS_msk                      (0x7f)
#define MANUFACTURERS_IDENTIFICATION_msk            (0x7f)
#define SYSTEM_EXCLUSIVE_MESSAGE_msk                (0x7f)
#define MIDI_TIME_CODE_QUARTER_FRAME_MESSAGE_D_msk  (0x70)
#define MIDI_TIME_CODE_QUARTER_FRAME_MESSAGE_T_msk  (0x0f) //non 0x7f, 0d127

//midi system message bit masks----------------------------------------------------
#define BEGIN_SYSTEM_EXCLUSIVE  (0b0000)
#define MIDI_TIME_CODE          (0b0001)
#define SONG_POSITION_POINTER   (0b0010)
#define SONG_SELECT             (0b0011)
#define TUNE_REQUEST            (0b0110)
#define END_SYSTEM_EXCLUSIVE    (0b0111)
#define TIMING_CLOCK            (0b1000)
#define START                   (0b1010)
#define CONTINUE                (0b1011)
#define STOP                    (0b1100)
#define ACTIVE_SENSING          (0b1110)
#define SYSTEM_RESET            (0b1111)

//PORT AND PIN DEFINES----------------------------------------------------------------
//Port Definitions
#define PORT_LED         GPIOC
//Pin Definitions
#define PIN_LED_RED      GPIO_PIN_6
#define PIN_LED_BLUE     GPIO_PIN_7
#define PIN_LED_ORANGE   GPIO_PIN_8
#define PIN_LED_GREEN    GPIO_PIN_9

//FREQUENCY DEFINES--------------------------------------------------------------------
#define FREQ_DEFAULT 60
#define FREQ_C2     65  //0  C
#define FREQ_Db2    69  //1  Db
#define FREQ_D2     73  //2  D
#define FREQ_Eb2    78  //3  Eb
#define FREQ_E2     82  //4  E
#define FREQ_F2     87  //5  F
#define FREQ_Gb2    93  //6  Gb
#define FREQ_G2     98  //7  G
#define FREQ_Ab2    104 //8  Ab
#define FREQ_A2     110 //9  A
#define FREQ_Bb2    117 //10 Bb
#define FREQ_B2     123 //11 B
//OTHER DEFINES
#define ON          1 //ON
#define OFF         0 //OFF

//global variables and structs------------------------------------------------------------------
static uint16_t index;
typedef struct {
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
} MIDI;

//------------------------------------------------------------------------------
static uint16_t inline get_statuscode(char data[])
{
    return ((data[index] & MESSAGETYPE_msk) >> 4);
}

//helper functions--------------------------------------------------------------

static void channel_mode_messages_handler(char data[], MIDI midi){ //inline????
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
static void system_message_handler(char data[], MIDI midi){
    uint16_t message_type = data[index] & 0x0f;
    switch(message_type)
    {
        case BEGIN_SYSTEM_EXCLUSIVE:
            //printf("BEGIN_SYSTEM_EXCLUSIVE\n");
            break;
        case MIDI_TIME_CODE:
            //printf("MIDI_TIME_CODE\n");
            break;
        case SONG_POSITION_POINTER:
            //printf("SONG_POSITION_POINTER\n");
            break;
        case SONG_SELECT:
            //printf("SONG_SELECT\n");
            break;
        case TUNE_REQUEST:
            //printf("TUNE_REQUEST\n");
            break;
        case END_SYSTEM_EXCLUSIVE:
            //printf("END_SYSTEM_EXCLUSIVE\n");
            break;
        case TIMING_CLOCK:
            //printf("TIMING_CLOCK\n");
            break;
        case START:
            //printf("START\n");
            break;
        case CONTINUE:
            //printf("CONTINUE\n");
            break;
        case STOP:
            //printf("STOP\n");
            break;
        case ACTIVE_SENSING:
            //printf("ACTIVE_SENSING\n");
            break;
        case SYSTEM_RESET:
            //printf("SYSTEM_RESET\n");
            break;
    }
    index++;
}

//inline functions--------------------------------------------------------------
MIDI set_channel(char data[], MIDI midi)
{
    char midi_channel = (data[index] & MESSAGETYPE_msk);

    if (midi_channel > 8 || midi_channel < 0)
    {
        midi.channel = CHANNEL1; // default channel if channel is out of range
    }
    else
    {
        midi.channel = midi_channel;
    }
    index++;

    return midi;
}
MIDI set_keynumber(char data[], MIDI midi)
{
    midi.keynumber = (data[index] & KEYNUMBER_msk);
    index++;
    return midi;
}
MIDI set_velocity(char data[], MIDI midi)
{
    midi.velocity = (data[index] & VELOCITY_msk);
    index++;
    return midi;
}
MIDI set_forceonkey(char data[], MIDI midi)
{
    midi.forceonkey = (data[index] & FORCEONKEY_msk);
    index++;
    return midi;
}
MIDI set_addressofcontrol(char data[], MIDI midi)
{
    midi.addressofcontrol = (data[index] & ADDRESS_OF_CONTROL_msk);
    index++;
    return midi;
}
MIDI set_valueofcontroloutput(char data[], MIDI midi)
{
    midi.valueofcontroloutput = (data[index] & VALUE_OF_CONTROL_OUTPUT_msk);
    index++;
    return midi;
}
MIDI set_programmeselect(char data[], MIDI midi)
{
    midi.programmeselect = (data[index] & PROGRAMME_SELECT_msk);
    index++;
    return midi;
}
MIDI set_pressurevalue(char data[], MIDI midi)
{
    midi.pressurevalue = (data[index] & PRESSURE_VALUE_msk);
    index++;
    return midi;
}
MIDI set_pitchbendlsb(char data[], MIDI midi)
{
    midi.pitchbendlsb = (data[index] & PITCH_BEND_LSB_msk);
    index++;
    return midi;
}
MIDI set_pitchbendmsb(char data[], MIDI midi)
{
    midi.pitchbendmsb = (data[index] & PITCH_BEND_MSB_msk);
    index++;
    return midi;
}

//note we start at C2 65hz if less then return 60hz
uint16_t midi_note_get_frequency(uint16_t key_num){
    key_num -= 36; //shift to C2 from C0 (12 notes * 3 octives = 36)
    if(key_num > 0){return FREQ_DEFAULT;} 
    
    //funciton globals
    uint16_t octave_num = 0;

    //make shift modulo x %= 12 to get notes A-G and octive
    while (key_num >= 12){
        key_num -= 12;
        octave_num++;
    }

    //Starting at Octave 2
    switch(key_num) {
        case 0:  return (FREQ_C2  << octave_num);
        case 1:  return (FREQ_Db2 << octave_num);
        case 2:  return (FREQ_D2  << octave_num);
        case 3:  return (FREQ_Eb2 << octave_num);
        case 4:  return (FREQ_E2  << octave_num);
        case 5:  return (FREQ_F2  << octave_num);
        case 6:  return (FREQ_Gb2 << octave_num);
        case 7:  return (FREQ_G2  << octave_num);
        case 8:  return (FREQ_Ab2 << octave_num);
        case 9:  return (FREQ_A2  << octave_num);
        case 10: return (FREQ_Bb2 << octave_num);
        case 11: return (FREQ_B2  << octave_num);
        default: return FREQ_DEFAULT;
    }
}

void setup_midi(){
    // ==== SAMPLE TIMER ====
    // sample_timer_register_cb(sample_timer_handler); // Register the Sample Timer Callback
    sample_timer_init();
    
    // // ==== OUTPUT CHANNELS ====
    // // Channels 1 - 4
    channel1_4_timer_init();

    // // Channel 1 Settings
    channel1_4_enable(CHANNEL1);
    channel1_4_set_waveform(CHANNEL1, WAVEFORM_SINE);
    channel1_4_on_off(CHANNEL1, OFF);

    // // Channel 2 Settings
    channel1_4_enable(CHANNEL2);
    channel1_4_set_waveform(CHANNEL2, WAVEFORM_TRIG);
    channel1_4_on_off(CHANNEL2, 1);

    // // Channel 3 Settings
    channel1_4_enable(CHANNEL3);
    channel1_4_set_waveform(CHANNEL3, WAVEFORM_RAMP);
    channel1_4_on_off(CHANNEL3, 1);

    // // Channel 4 Settings
    channel1_4_enable(CHANNEL4);
    channel1_4_set_waveform(CHANNEL4, WAVEFORM_SQUARE);
    channel1_4_on_off(CHANNEL4, 1);

    // // Start the sample timer (advance the sampled waveforms)
    sample_timer_start();
}

//main function------------------------------------------------------------------
void set_midi(char data[]) 
{
    MIDI midi;

    while(data[index] != 0x00)
    {
        switch(get_statuscode(data))
        {
            case SYSTEM_MESSAGE:
                system_message_handler(data, midi);
                //TODO
                break;
            case NOTE_ON_EVENT: 
                midi = set_channel(data, midi);
                midi = set_keynumber(data, midi);
                midi = set_velocity(data, midi);

                channel1_4_on_off(midi.channel,ON);
                channel1_4_frequency(midi.channel, 
                    midi_note_get_frequency(midi.keynumber));
                channel1_4_volume(CHANNEL1,midi.velocity);

                //printf("NOTE_ON_EVENT: \n\tchannel:0x%02X\n\tKey Number:0x%02X\n\tvelocity:0x%02X\n\n", channel, keynumber, velocity); 
                break;
            case NOTE_OFF_EVENT: 
                midi = set_channel(data, midi);
                midi = set_keynumber(data, midi);
                midi = set_velocity(data, midi);

                channel1_4_on_off(midi.channel, OFF);
                channel1_4_frequency(midi.channel, 
                                    midi_note_get_frequency(midi.keynumber));
                channel1_4_volume(CHANNEL1, midi.velocity);

                //printf("NOTE_OFF_EVENT: \n\tchannel:0x%02X\n\tKey Number:0x%02X\n\tvelocity:0x%02X\n\n", channel, keynumber, velocity); 
                break;   
            case POLYPHONIC_KEY_PRESSURE:
                set_channel(data, midi);
                set_keynumber(data, midi);
                set_forceonkey(data, midi);
                //printf("POLYPHONIC_KEY_PRESSURE: \n\tchannel:0x%02X\n\tKey Number:0x%02X\n\tforceonkey:0x%02X\n\n", channel, keynumber,forceonkey); 
                break;
            case CONTROL_CHANGE:
                set_channel(data, midi);

                channel_mode_messages_handler(data, midi); //

                set_addressofcontrol(data, midi);
                set_valueofcontroloutput(data, midi);
                //printf("CONTROL_CHANGE: \n\tchannel:0x%02X\n\taddress of control:0x%02X\n\tvalue of controloutput:0x%02X\n\n", channel, addressofcontrol, forceonkey); 
            case PROGRAM_CHANGE: 
                set_channel(data, midi);
                set_programmeselect(data, midi);
                //printf("PROGRAM_CHANGE \n\tchannel:0x%02X\n\taddress of control:0x%02X\n\n", channel,programmeselect); 
                break;
            case CHANNEL_PRESSURE: 
                set_channel(data, midi);
                set_pressurevalue(data, midi);
                //printf("PROGRAM_CHANGE: \n\tchannel:0x%02X\n\tpressurevalue:0x%02X\n\n", channel, pressurevalue); 
                break;
            case PITCH_BEND: 
                set_channel(data, midi);
                set_pitchbendlsb(data, midi);
                set_pitchbendmsb(data, midi);
                //printf("PITCH_BEND: \n\tchannel:0x%02X\n\tpitch bend lsb_msk:0x%02X\n\tpitch bend msb_msk:0x%02X\n\n", channel, pitchbendlsb, pitchbendmsb); 
                break;
        }
    }
}