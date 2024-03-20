/*
 Title:		main.c
 Author:	Badre
 Created:	20/02/2024
 Last Updated: 20/03/2024

 Modinfo: 
 23/03/2024		Update code for ascii > 128
 				added fonction restFont and createAscii 
*/
#include <stdio.h>
#include <mos_api.h>
#include <agon/vdp_vdu.h>
#include "agon_timer.h"
#include "progress.h"
#include "conio_agon.h"

void resetFont();
void createAscii(uint8_t c, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8);


int main() {   
    int total = 200;
    int step = 0;

    setmode(scr_mode);
    clrscr();    
    createAscii(129, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    cursorEnable(false);
    textattr(BLACK, BRIGHT_GREEN);   
    gotoxy(0, 0);
    printf("Files : %d\r\n", total);
    while ( step < total )
    {
        //do some action
        delayms(10);
        step += 2;
        DoProgress( "Progress:", step, total, '#' );
    }
    printf("\r\nPress any key to continue ...\r\n");
    getch();
    clrscr();            
    gotoxy(0, 0);
    step = 0;
    printf("Files : %d\r\n", total);
    while ( step < total )
    {
        //do some action
        delayms(10);
        step += 2;
        DoProgress( "Progress:", step, total, 129 );
    }
    printf("\r\nPress any key to continue ...\r\n");
    getch();
    clrscr();
    loadingPercent(0, 0, 100, "Loading..." );

    printf("\r\nPress any key to continue ...\r\n");
    getch();
    clrscr();
    loadingSpinner(0, 0, 100, "Loading...");

    crlf();
    cursorEnable(true);
    textattr(BLACK, BRIGHT_WHITE);
    resetFont();
    return 0;
}

//Reset font default 
void resetFont()
{
    putch(23);
    putch(0);
    putch(0x91);
}

//Change ascii caracther from ascii code
void createAscii(uint8_t c, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8)
{
    putch(23);
    putch(c);
    putch(b1);
    putch(b2);
    putch(b3);
    putch(b4);
    putch(b5);
    putch(b6);
    putch(b7);
    putch(b8);
}