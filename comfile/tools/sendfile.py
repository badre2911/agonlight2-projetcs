## Title:       sendfile.py
## Author:      Jeroen Venema
## Created:     25/10/2022
## Last update: 11/09/2023

## syntax
## send.py FILENAME <PORT> <BAUDRATE>
## 

## Modinfo:
## 25/10/2022 initial version
## 10/09/2023 Script converts binary file to Intel Hex during transmission. 
##            Using defaults as constants.
## 11/09/2023 Wait time variable introduced for handling PC serial drivers with low buffer memory
## 18/02/2024 Modified by badre, removed code regarding hex files, any type of file can be sent

#DEFAULT_START_ADDRESS = 0x40000
DEFAULT_SERIAL_PORT   = 'COM17'
DEFAULT_BAUDRATE      = 115200
DEFAULT_LINE_WAITTIME = 0       ## A value of +/- 0.003 Helps PC serial drivers with low buffer memory

def errorexit(message):
  print(message)
  print('Press ENTER to continue')
  input()
  exit()
  return

import sys
import time
import os.path
import tempfile
import serial.tools.list_ports
try:
  import serial
except ModuleNotFoundError:
  errorexit('Please install the \'pyserial\' module with pip')


if len(sys.argv) == 1 or len(sys.argv) >4:
  sys.exit('Usage: send.py FILENAME <PORT> <BAUDRATE>')

if not os.path.isfile(sys.argv[1]):
  sys.exit(f'Error: file \'{sys.argv[1]}\' not found')

try:
    print('Launch comfile -f <filename> to Agon before continuing')
    print('When you are ready, press any key and agon and press Enter key for launch transfert\r\n')
    input("Press Enter to continue or CTRL+C for quit\r\n")
except KeyboardInterrupt:
    print('Program terminated !\r\n')
    exit(1)
    
if len(sys.argv) == 2:
  #serialport = DEFAULT_SERIAL_PORT
  serialports = serial.tools.list_ports.comports()
  if len(serialports) > 1:
    sys.exit("Multiple COM ports - cannot automatically select");
  serialport = str(serialports[0]).split(" ")[0]
if len(sys.argv) >= 3:
  serialport = sys.argv[2]

if len(sys.argv) == 4:
  baudrate = int(sys.argv[3])
else:
  baudrate = DEFAULT_BAUDRATE

# report parameters used
print(f'Sending \'{sys.argv[1]}\' ', end="")
print(f'Using serial port {serialport}')
print(f'Using Baudrate {baudrate}')

file = open(sys.argv[1], "rb")
content = file.read()

ser = serial.Serial()
ser.baudrate = baudrate
ser.port = serialport
ser.setDTR(False)
ser.setRTS(False)

if ser.isOpen():
    ser.close()
try:
    ser.open()
    print('Opening serial port...')
    time.sleep(1)
    print('Sending data...')

    ser.write(content)
    ser.close()
except serial.SerialException:
    errorexit('Error: serial port unavailable')

file.close()
