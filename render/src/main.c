/*
 Title:		main.c
 Author:	Badre
 Created:	25/03/2024
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "font.h"

const uint8_t char_display = '.';

void usage();
void render(const uint8_t *bitmap, FontDef_t *tab_font);

int main(int argc, char **argv) {
    int ord;
    FontDef_t *font;
    font = &Font_vga_16; //Font used
    
    if (argc != 2) {
        usage();
        return -1;
    }

    ord = atoi(argv[1]);
    if (ord > 255 || ord < 0) { // num font must be > 0 and < 255
        usage();
        return -1;
    }

    if( ord > font->len) {
        usage();
        return -1;
    }
    const uint8_t *bitmap = &font->data[ord*font->FontHeight];
    
    render(bitmap, font);
    printf("\r\n");
    return 0;
}

void render(const uint8_t *bitmap, FontDef_t *tab_font) {
    int x,y;
    int set;    
    for (x=0; x < tab_font->FontHeight; x++) {
        for (y=0; y < tab_font->FontWidth; y++) {
            set = bitmap[x] & 1 << y;
            printf("%c", set ? char_display : ' ');
        }
        printf("\n");
    }
}

void usage() {
    printf("Usage: render <char_code in decimal>\n");
    printf("Example: render 65  <char_code> Decimal character code between 0 and 255\n");
}