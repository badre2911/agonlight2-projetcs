# agonlight2-projetcs
Utilities for [agonlight2](https://www.olimex.com/Products/Retro-Computers/AgonLight2/open-source-hardware)

The projects are built with VSCODE AgDev on windows and binaries are provided, that you can put in the /mos directory of the SD-card. Version of MOS and VDP are, console8 MOS 2.1.0 and VDP 2.3.0 or upper, not tested with previous versions.

In some assembler routines I refer to mos_api.inc via a path relative to where your project is located.
Don't forget to change this path relative to where your source is located: `include '../../lib/libc/mos_api.inc'`  

>Default projet are on folder CEdev/AgExamples/ (MyProject/src, MyProject/bin)

Link for AgDev and instructions for installation [AgDev](https://github.com/pcawte/AgDev).


### scani2c
allows you to scan the i2c bus of a module connected to the UEXT connector. The i2c address is displayed if an module i2c is found.

launch scani2c from mos directory.  
Example command line: `scani2c`

output:

`I2C bus scanning`

`Address 0x3C`


### send
allows to send file to RS232 (UEXT). Escape Key to cancel transfert.  
Default BAUD is 115200  
Put a file to /mos directory  
Command line: `send <filename>`    on Agon
              `rcvfile <filename>  on PC hosts (batch file wiht python script)

### dumpview
allows to dump a file to screen.  
Example command line: `dumpview <filename>`  
show readme file from dumpview for more information

### ascii
allows show ascii table.  
Put a file to /mos directory  
Example command line: `ascii`

### atconfig

allow configure wifi or restore to default factory the esp8266. Default baud is 115200.  
Show readme file from folder atconfig for more informations.

### comfile  
Allow to transfert binary or text file from PC to Agon  
Show readme file from folder comfile for more informations.

### pause  
allows to pause on batch file.  
Put a file to /mos directory  
Example command line: `pause`  
Result: `Press any key to continue ...`

### textprogress  

Sample code that show progress bar on text mode (progress bar, spinner ...)  
Put a file txtprogress.bin to /mos directory  
Example command line: `txtprogress`

### shell  

Alow to show how call shell mos from c code.  
Put a file on /mos directory
Command line: `shell`  

### crc32  

Allow generate the CRC-32 value for the file named in the command line argument.  
Put a file on /mos directory  
Command line: `crc32 <filename>`

### MD5

MD5 Code adapted for AgonLight from Zunawe (https://github.com/Zunawe/md5-c),
forked by MilchRatchet  (https://github.com/MilchRatchet/md5-c)  

Allow calculate MD5 from string or file.  
Put a file on /mos directory  

**Command line:**  
`MD5 <string>   ex: MD5 Hello`  
`MD5 <filename> ex: MD5 myfile.txt`
