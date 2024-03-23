## COMFILE
Allow transfert file from PC to Agonlight2 binary or text file.  
Limited file to transfert is 256k.  
Default baud is 115200.  
transfert use a script `sendfile.py` and `sendfile.bat` for transfert file from PC to Agon

```
Put a file comfile.bin to /mos directory  
Download sendfile.py and sendfile.bat (folder tools) on your desktop in directory where the file to transfert  
Launch comfile.bin on the Agon Light side and follow instructions  
Launch sendfile.bat yourfile.txt COM2 for example
```

**Hardware**  

I use MOD-RS232 D-Sub 9pin extension board,UEXT Interface: RS232 OLIMEX.  
You can also use a USB RS232 TTL 3.3v adapter by connecting the GND, RX, TX of the serial USB cable to the GND TX, RX of the UEXT connector directly.  

**Usage**  

Command line:  
`comfile -f myfile.txt` on Agon  
`sendfile myfile.txt COM2`on PC host

```
Usage: comfile -f <filename> [OPTIONS]
-h this help	
-b <baud> (default is 115200)	
-t <timeout> (default is 3000)
-f <filename>
Example: comfile -f myfile.txt
Example: comfile -f myfile.txt -b 57600
Example: comfile -f myfile.txt -b 57600 -t 4000
```

**Tools for Upload**  

On folder tools you have a script sendfile.py and .bat file sendfile.bat  
Launch transfert with command line: `sendfile myfile.txt COM2`  
Don't forget to modify your COM port.  

First, launch comfile on Agon then when you are ready to send the file which is on your PC,  
Launch `sendfile myfile.txt COM2` on your PC host,
When you are ready, press a key on the Agon keyboard or ESC to cancel for waiting transfert,  
Press  Enter key on you PC host for launch transfert to Agon








