
SET CEDEV_PATH=D:\CEdev
%CEDEV_PATH%\bin\fasmg.exe -v1  "%CEDEV_PATH%\meta\ld.alm" -i "DEBUG := 0" -i "PROG_NAME := '%1'" -i "HAS_LIBC := 0" -i "HAS_LIBCXX := 0" -i "PREFER_OS_CRT := 0" -i "PREFER_OS_LIBC := 0" -i "HAS_EXIT_HANDLER := 0" -i "HAS_ARG_PROCESSING := 0"  -i "include '%CEDEV_PATH%\\meta\\linker_script'"  -i "range .bss $060000 : $09FFFF" -i "provide __stack = $0AFFFF" -i "locate .header at $0B0000" -i map -i "source 'src\\%1.asm','%CEDEV_PATH%\\lib\\agon\\intagon.src', '%CEDEV_PATH%\\lib\\agon\\vdp_vdu.c.src', '%CEDEV_PATH%\\lib\\agon\\vdp_key.c.src'"  bin\%1.bin
SET CEDEV_PATH=
