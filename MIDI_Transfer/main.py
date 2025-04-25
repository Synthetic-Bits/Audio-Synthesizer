#
# main.py
#
# This script reads the given MIDI file and sends it over UART.
#
# Author:  Kenneth Gordon
# Date:  April 24, 2025
#

# Python imports
import serial
import time
import io
from contextlib import redirect_stdout

from mido import MidiFile, bpm2tempo
from mido.ports import BaseOutput

# Global variables
SONG_PATH = "./songs/bones.mid"
UART_PORT = "COM12"
BPM = 120

class UARTPort(BaseOutput):
    '''
    Description
    -----------
    This object is a custom mido output port.  It uses UART as an output.
    '''

    # Object variables
    serial_port = None

    def __init__(self, port, baud_rate):
        '''
        Description
        -----------
        Opens the mido port on the given UART peripheral.

        Parameters
        ----------
        port - the port that the UART peripheral is on. (IE: COM5)
        baud_rate - the baud rate of the UART peripheral.

        Returns
        -------
        A Mido port that uses the UART peripheral.
        '''

        # Open the UART port and then call the super-constructor
        self.serial_port = serial.Serial(port, baud_rate)
        super().__init__()
        
    def _close(self):
        '''
        Description
        -----------
        Closes the UART port.

        Parameters
        ----------
        None.

        Returns
        -------
        None.
        '''

        # Close the UART port
        self.serial_port.close()

    def _send(self, message):
        '''
        Description
        -----------
        Sends the given MIDI message through the output port.

        Parameters
        ----------
        message - the MIDI message to send.

        Returns
        -------
        None.
        '''

        print(f'{message} : {message.bytes()}')
        self.serial_port.write(message.bytes())

def main():
    '''
    Description
    -----------
    Main application loop that sends MIDI data to the synthesizer through UART.

    Parameters
    ----------
    None.

    Returns
    -------
    None.
    '''

    port = UARTPort(UART_PORT, 31250)
    mid = MidiFile(filename=SONG_PATH, clip=True)

    # Re-write the tempo
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'set_tempo':
                msg.tempo = bpm2tempo(BPM)  # New BPM
                break  # Exit after first tempo found
    
    time.sleep(1)
    print("Starting MIDI!")

    log = io.StringIO() # Prevent Printing
    with redirect_stdout(log):
        sent_messages = 0
        for msg in mid.play():
            sent_messages += 1
            port.send(msg)
        port.lose()

    print(f"We sent a total of {sent_messages} messages!")

if __name__ == "__main__":
    main()