#!/usr/bin/env python3

import os
import sys
import serial.tools.list_ports
import time
import subprocess

# Title:       agon_transfer.py
# Author:      Tom Schimana
# Created:     31/12/2023
# Last update: 01/01/2024

# Script to automate the transfer of BASIC programs to Agon light.
# Requires 'SET CONSOLE 1' enabled, a USB-to-UART connection to Agon ESP32, and 'hexload'.

# Usage: agon_transfer.py FILENAME

# Your Serial Device address for USB-to-UART connection to Agon
DEVICE = "COM17"

# Check if a filename is provided
if len(sys.argv) != 2:
    print("Usage: {} filename".format(sys.argv[0]))
    sys.exit(1)

FILENAME = sys.argv[1]

# Function for sending commands with a fixed delay between each character
def send_command(command, command_delay=0.5):
    char_delay = 0.03  # Fixed delay between characters
    with serial.Serial(DEVICE, 115200, timeout=1) as ser:
        for char in command + "\r":
            ser.write(char.encode())
            time.sleep(char_delay)
        time.sleep(command_delay)

# Exit BASIC, initiate Hexload
#send_command("*BYE")
#send_command("hexload uart1 115200 " + FILENAME)
time.sleep(2)
send_command("hexload vdp " + FILENAME)

# `send.py` is part of Hexload
send_py_path = os.path.expanduser("send.py")
#subprocess.run(["python3", send_py_path, FILENAME, "/dev/tty.PL2303G-USBtoUART10", "115200"])
time.sleep(3)
subprocess.run(["python", send_py_path, FILENAME, "COM17"])
time.sleep(1)

# Return to BBC BASIC and load the program
#send_command("load /bbcbasic.bin")
#send_command("run")
#send_command("LOAD \"" + FILENAME + "\"", 1.0)
#send_command("RUN", 0)

