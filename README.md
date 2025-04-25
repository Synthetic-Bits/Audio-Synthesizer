# MIDI Digital Synthesizer

The MIDI Digital Synthesizer is an embedded system capable of interpreting MIDI input signals and synthesizing the audio for the MIDI input. The project was developed as a final project for the University of Utah's ECE 5780: Embedded Systems class. While originally planned to be developed on an STM32F072 Discover board, the system now only works with a STM32H533RE NUCLEO board due to limitation's with the Discovery's flash memory as well as clock speed.

Code for the STM32F072 is present within this repository however, it should be noted that this code hasn't been integrated together and thus, doesn't really work in a cohesive system. That said, the code did represent effort that the Synthetic Bits put in to creating this project and thus, was included in this repository.

## Group Members

- Kenneth Gordon
- Bryant Watson
- Hayoung Im
- Adrian Sucahyo

## Features

The MIDI Digital Synthesizer has the following features:

### Hardware

Microcontroller:
 - STM32H533RE (NUCLEO)

Channels:
 - 7 PWM Channels
     - Sine
     - Triangle
     - Ramp
     - Square
 - 1 DAC
     - Noise

Individual Channel Control:
 - Gain Control
 - Toggleable Low Pass Filter

Master Control:
 - Master Volume Control

Output Speaker Driver:
 - LM1875 Based Class AB Amplifier
 - Up to 20 Watts Driver Capability

### Software

MIDI Interface:
 - MIDI Input
 - MIDI Output

Configurable Channel Waveforms
 - Sine
 - Triangle
 - Ramp
 - Square
 - Noise (Restricted to Channel 8)

Configurable Voice Channels
- Channels 1-7 capable of 8 voices
- Channels 8 capable of 1 voice

## Setup Guide

1. Put the NUCLEO on the PCB.
2. Hook up the +-15V power supply as well as the 8 Ohm speaker to the board.
3. Configure your low-pass filters as desired.
4. Connect MIDI (either with the keyboard or through the pin headers)
5. Flash the NUCLEO.
6. Send MIDI to the board. Mix each of the channels as well as the master channel as desired.
7. _Caution_, the class AB amplifier gets really hot, really quickly. Don't burn yourself or the board!

Maybe a flow-diagram of the process would be nice here?

Showcase what needs to be connected on a picture of the PCB?

## Board 

### Front Panel

The leftmost connector on the board is the 8 Ohm speaker connector. The left port of this connector is the negative out and the right port is positive out. Refer to the figures above for wire connections.

The middle four connectors on the board control the positive and negative voltage inputs. The connector ports have the following order: GND, -15V, GND, and +15V. 

The rightmost two connectors are the MIDI IN and MIDI OUT connectors. The MIDI IN is used for sending midi commands to the microcontroller for synthesis. The MIDI IN line can connect to a compatible keyboard or computer for sound synthesis. The MIDI OUT is current not used but was included as future code can be written to send MIDI data to other devices from the synthesizer.

![Front Panel Diagram](./Documentation/doc_imgs/Synthesizer-Front_panel.svg)

![Front Panel Img](./Documentation/doc_imgs/Synthesizer-Front_panel.jpeg)

### Top Down

The rightmost device is the the microcontroller board.  This board connects with the main synthesizer board. The blue dots are where a jumper is set.  The E5V connector is used to let the board know to use the external power. All other jumpers are standard and come with the NUCLEO board and the synthesizer board. The NUCLEO board contains a reset button to reset the program and a User button that is currently not used for the synthesizer. Furthermore, the programming for the NUCLEO board uses a USB type C connector on the bottom of the STM32 board. If the user wants to use USB power for testing or MIDI-OUT the user can change the jumper to 5V-STLINK.

The top left section contains Channel 1-8 models. This module is a low-pass filter and amplifier buffer. The ON/OFF controls the low-pass filter. There is a Volume nob that uses a POT. If the more expensive POTS are not available cheap POTs can be used. If cheaper pots are used the jumper must be moved down. 

There is a master channel that mixes the above 8 channels and applies an amplifier buffer. There is also a Volume nob using a POT.

Other Pins on this board were not mentioned above. Some of these are not Jumpers but Debug ports such as the pins near the capacitors near the +15V and -15V.

On the bottom right there are 4 pins near the MIDI IN that can also be used to send MIDI data from a computer using UART. These pins are labeled 3V, RX, TX, and GND.

![Front panel Diagram](./Documentation/doc_imgs/Synthesizer-top_panel.svg)
![Front panel Img](./Documentation/doc_imgs/Synthesizer-top_panel.png)

## Board Schematic and PCB

### Schematics
![Front panel Diagram](./Documentation/doc_imgs/MIDI-Synthesizer-schematic.png)
![Front panel Diagram](./Documentation/doc_imgs/MIDI-Synthesizer-schematic2.png)

### PCB Design
![Front panel Diagram](./Documentation/doc_imgs/MIDI-Synthesizer-PCB.png)

## Temporary

This section is notes for us so we don't forget to include things in this document/the rest of the documentation.

Basic instructions on how to set it up
Wiring diagrams
Schematics
Flow charts
Make sure it's organized and written such that it could be of use to an internet stranger interested in the project.
