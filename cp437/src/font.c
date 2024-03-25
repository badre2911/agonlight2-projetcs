/*
 Title:		font_cp437.c
 Author:	Badre
 Created:	23/03/2024
*/
#include <stdio.h>
#include <mos_api.h>
#include "font.h"

#define FONT(a, b) (uint8_t)(a[(b)])
#define FONT2D(a, b, c) (uint8_t)(a[(b)][(c)])

//Set default font
void set_default_font()
{
    putch(23);
    putch(0);
    putch(0x91);
}

//Load font from 0-255
void load_font(FontDef_t fontdef)
{    
    int n;
    for(int i = 0; i < 255; i++) {        
        n = i * 8;
        putch(23);
        putch(0);
        putch(0x90);
        putch(i);         

        putch(FONT(fontdef.data, n));
        putch(FONT(fontdef.data, n+1));
        putch(FONT(fontdef.data, n+2));
        putch(FONT(fontdef.data, n+3));
        putch(FONT(fontdef.data, n+4));
        putch(FONT(fontdef.data, n+5));
        putch(FONT(fontdef.data, n+6));
        putch(FONT(fontdef.data, n+7));            
    }
}

//Redefines font from x  to y num font
void load_range_font(FontDef_t fontdef)
{                
    int n = 0;
    for(int num_font = fontdef.first_char; num_font < fontdef.last_char+1; num_font++) {                       
       
        putch(23);
        putch(0);
        putch(0x90);
        putch(num_font);         

        putch(FONT(fontdef.data, n));
        putch(FONT(fontdef.data, n+1));
        putch(FONT(fontdef.data, n+2));
        putch(FONT(fontdef.data, n+3));
        putch(FONT(fontdef.data, n+4));
        putch(FONT(fontdef.data, n+5));
        putch(FONT(fontdef.data, n+6));
        putch(FONT(fontdef.data, n+7));  
        n += 8;          
    }
}


//Redefine character n (0-255) with 8 bytes of data
bool redefine_num_font(uint8_t *array, uint8_t num)
{
    if(num < 0 || num > 255) return false;

    putch(23);
    putch(0);
    putch(0x90);
    putch(num);    
    
    putch(FONT(array, 0));
    putch(FONT(array, 1));
    putch(FONT(array, 2));
    putch(FONT(array, 3));
    putch(FONT(array, 4));
    putch(FONT(array, 5));
    putch(FONT(array, 6));
    putch(FONT(array, 7));  

    return true;  
}
