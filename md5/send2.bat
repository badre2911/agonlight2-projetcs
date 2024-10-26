Make clean
Make V=1
srec_cat bin\%1.bin -binary  -offset 0x0B0000 -o bin\%1.hex -intel
python agon_transfer.py bin\%1.hex