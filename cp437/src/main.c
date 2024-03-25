/*
 Title:		main.c
 Author:	Badre
 Created:	25/03/2024
*/
#include <stdio.h>
#include <string.h>
#include "font.h"

void usage();

int main(int argc, char *argv[])
{        
    if(argc > 2) {
        usage();
        return -1;
    }

    if(argc == 2) {
        if(strcmp((const char *)argv[1], (const char *)"reset") == 0) {
            set_default_font();
            printf("Font has been reset successfully.\r\n");
            return 0;
        } else {
            usage();
            return -1;
        }
    }

    load_range_font(Font_cp437_8x8);
    printf("\r\nFont CP437 load successfully.\r\n");
       
    return 0;
}

void usage()
{
    printf("Usage: cp437 [reset]\r\n");    
}