/*
 Title:		tools.c
 Author:	Badre
 Created:	23/12/2023
*/

#include <stdint.h>
#include <mos_api.h>
#include "tools.h"

void write16bit(uint16_t w)
{
	putch(w & 0xFF); // write LSB
	putch(w >> 8);	 // write MSB	
}

void write24bit(uint24_t w)
{
	putch(w & 0xFF); // write LSB
	putch(w >> 8);	 // write middle	
    putch(w >> 16);	 // write MSB	
}
