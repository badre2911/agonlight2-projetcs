/*
 Title:		main.c
 Author:	Badre
 Created:	25/03/2024
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "font.h"

void usage();
void render(const uint8_t *bitmap);

int main(int argc, char **argv) {
    int ord;
    if (argc != 2) {
        usage();
        return 1;
    }
    ord = atoi(argv[1]);
    if (ord > 127 || ord < 0) {
        usage();
        return 2;
    }
    const uint8_t *bitmap = &Font_basic_8x8.data[ord*8];
    
    render(bitmap);
    return 0;
}

void render(const uint8_t *bitmap) {
    int x,y;
    int set;    
    for (x=0; x < 8; x++) {
        for (y=0; y < 8; y++) {
            set = bitmap[x] & 1 << y;
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

void usage() {
    printf("Usage: render <char_code>\n");
    printf("       <char_code> Decimal character code between 0 and 127\n");
}