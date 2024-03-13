/*
 Title:		main.c
 Author:	Badre
 Created:	20/02/2024
*/
#include <stdio.h>
#include <mos_api.h>
#include "agon_timer.h"
#include "progress.h"
#include "conio_agon.h"



int main() {   
    int total = 200;
    int step = 0;

    clrscr();
    cursorEnable(false);
    textattr(BLACK, BRIGHT_GREEN);   
    gotoxy(0, 0);

    printf("Files : %d\r\n", total);
    while ( step < total )
    {
        //do some action
        delayms(10);
        step += 1;
        DoProgress( "Progress:", step, total, '#' );
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
    return 0;
}
