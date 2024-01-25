### atconfig

allow configure wifi or restore to default factory the esp8266. Default baud is 115200.  
Configuration is saved in flash. Key ESC allow quit without save. 

>Module esp8266 is conected to UEXT connector from agon (GND, TX, RX). I use Nodecmu ESP8266EX but should also work on an ESP32.  
You can use too Olimex module MOD-WIFI-ESP8066.

I used firmware ESP8266-IDF-AT_2.2.1 AT command [firmware 2.2.1.0](https://docs.espressif.com/projects/esp-at/en/release-v2.2.0.0_esp8266/AT_Binary_Lists/ESP8266_AT_binaries.html)  
Download Flash Download Tools [here](https://www.espressif.com/en/support/download/other-tools?keys=&field_type_tid%5B%5D=14)

Put a file to /mos directory

Example command line: `atconfig <baud>` default bauds is 115200 if not parameter.
