## FASM SAMPLE CODE

Example of assembly code with the AgDev fasmg compiler.  
The fasmg assembler is a little different than the ez80asm or ZDSII syntax.  

**functioning:**  
Create an **src** directory and create your assembler file inside this directory, for example test.asm.  
Create a **bin** directory if it does not exist.  
compile the assembly program using the **asm.bat** file.  

```
Don't forget to change the CDEV_PATH environment variable in the asm.bat file which is the directory
where AgDev is located.
```

Command line : `asm test`  

The test.bin file will be created in the bin directory.  

**asm.bat**  

```

SET CEDEV_PATH=D:\CEdev
%CEDEV_PATH%\bin\fasmg.exe -v1  "%CEDEV_PATH%\meta\ld.alm" -i "DEBUG := 0" -i "PROG_NAME := '%1'" -i "HAS_LIBC := 0" -i "HAS_LIBCXX := 0" -i "PREFER_OS_CRT := 0" -i "PREFER_OS_LIBC := 0" -i "HAS_EXIT_HANDLER := 0" -i "HAS_ARG_PROCESSING := 0"  -i "include '%CEDEV_PATH%\\meta\\linker_script'"  -i "range .bss $060000 : $09FFFF" -i "provide __stack = $0AFFFF" -i "locate .header at $0B0000" -i map -i "source 'src\\%1.asm','%CEDEV_PATH%\\lib\\agon\\intagon.src', '%CEDEV_PATH%\\lib\\agon\\vdp_vdu.c.src', '%CEDEV_PATH%\\lib\\agon\\vdp_key.c.src'"  bin\%1.bin
SET CEDEV_PATH=
```
In this sample code, header address is $0B0000, this program need to put on /mos directory and launched directly. 
If you change header address to $40000 your have to run your progam with  

```
load test.bin
run
```
