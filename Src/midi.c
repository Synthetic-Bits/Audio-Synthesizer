#include <stdint.h>
#include <stdio.h>
#include <stm32f0xx_hal.h>

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

//PORT AND PIN DEFINES-----------------------------------------------------------------------
//Port Definitions
#define PORT_LED         GPIOC
//Pin Definitions
#define PIN_LED_RED      GPIO_PIN_6
#define PIN_LED_BLUE     GPIO_PIN_7
#define PIN_LED_ORANGE   GPIO_PIN_8
#define PIN_LED_GREEN    GPIO_PIN_9

//global variables------------------------------------------------------------------
static uint16_t index;

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
    midi.channel = (data[index] & MESSAGETYPE_msk);
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


//main function------------------------------------------------------------------
MIDI get_midi(char data[]) 
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
                if(midi.keynumber == 0x24) {
                    HAL_GPIO_WritePin(PORT_LED, PIN_LED_RED, GPIO_PIN_SET);
                    //Red LED ON
                }
                else if (midi.keynumber == 0x25){
                    HAL_GPIO_WritePin(PORT_LED, PIN_LED_GREEN, GPIO_PIN_SET);
                    //Greed LED ON
                }
                else if (midi.keynumber == 0x26){
                    HAL_GPIO_WritePin(PORT_LED, PIN_LED_BLUE, GPIO_PIN_SET);
                    //Blue LED ON
                }
                else if (midi.keynumber == 0x27){
                    HAL_GPIO_WritePin(PORT_LED, PIN_LED_ORANGE, GPIO_PIN_SET);
                    //Orange LED ON
                }
                //printf("NOTE_ON_EVENT: \n\tchannel:0x%02X\n\tKey Number:0x%02X\n\tvelocity:0x%02X\n\n", channel, keynumber, velocity); 
                break;
            case NOTE_OFF_EVENT: 
                midi = set_channel(data, midi);
                midi = set_keynumber(data, midi);
                midi = set_velocity(data, midi);
                if(midi.keynumber == 0x24) {
                    HAL_GPIO_WritePin(PORT_LED, PIN_LED_RED, 0);
                }
                else if (midi.keynumber == 0x25){
                    HAL_GPIO_WritePin(PIN_LED_GREEN, PIN_LED_RED, 0);
                    //Greed LED ON
                }
                else if (midi.keynumber == 0x26){
                    HAL_GPIO_WritePin(PIN_LED_BLUE, PIN_LED_RED, 0);
                    //Blue LED ON
                }
                else if (midi.keynumber == 0x27){
                    HAL_GPIO_WritePin(PIN_LED_ORANGE, PIN_LED_RED, 0);
                    //Orange LED ON
                }
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

    return midi;
}