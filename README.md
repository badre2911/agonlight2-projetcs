# agonlight2-projetcs
Utilities for [agonlight2](https://www.olimex.com/Products/Retro-Computers/AgonLight2/open-source-hardware)

The projects are built with AgDev on windows and binaries are provided, that you can put in the /mos directory of the SD-card. Version of MOS and VDP are, MOS 2.1.0 and VDP 2.3.0, not tested with previous versions.

Link for AgDev and instructions for installation [AgDev](https://github.com/pcawte/AgDev).


### scani2c
allows you to scan the i2c bus of a module connected to the UEXT connector. The i2c address is displayed if an module i2c is found.

launch scani2c from mos directory.

Example command line: `scani2c`

output:

`I2C bus scanning`

`Address 0x3C`


### send
allows to send file to RS232 (UEXT)

Put a file to /mos directory

Example command line: `send <filename>`

### dumpview
allows to dump a file to screen.

Example command line: `dumpview <filename>`

show readme file from dumpview for more information

