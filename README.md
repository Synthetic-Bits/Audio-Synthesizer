# MIDI Digital Synthesizer

The MIDI Digital Synthesizer is an embedded system capable of synthesizing audio.  The synthesizer is able to interpret MIDI input signals and generate the desired waveforms from the MIDI input.

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
- Channels 1-4 capable of 4 voices
- Channels 5-8 capable of 1 voice

## Setup Guide

1. Put the NUCLEO on the PCB.
2. Hook up the +-15V power supply as well as the 8 Ohm speaker to the board.
3. Configure your low-pass filters as desired.
4. Connect MIDI (either with the keyboard or through the pin headers)
5. Flash the NUCLEO.
6. Send MIDI to the board.  Mix each of the channels as well as the master channel as desired.
7. _Caution_, the class AB amplifier gets really hot, really quickly.  Don't burn yourself or the board!

Maybe a flow-diagram of the process would be nice here?

Showcase what needs to be connected on a picture of the PCB?

## Temporary

This section is notes for us so we don't forget to include things in this document/the rest of the documentation.

Basic instructions as well as setup guide
Wiring diagrams
Schematics
Flow charts

Give the project name, its purpose, functionality, basic instructions on how to set it up, wiring diagrams, schematics, flow charts. Make sure it's organized and written such that it could be of use to an internet stranger interested in the project.