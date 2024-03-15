/*
 Title:		main.c
 Author:	Badre
 Created:	15/03/2024
*/
#include <stdio.h>
#include <mos_api.h>
#include "agon_timer.h"
#include "conio_agon.h"

extern char shell(const char * cmd);


int main() {   
    setmode(scr_mode);   
    clrscr();   
    textattr(BLACK, BRIGHT_GREEN);   
    gotoxy(0, 0);
    printf("Press anykey for call shell mos with oscli in c\r\n");
    getch();
    shell("dir");
    crlf();   
    textattr(BLACK, BRIGHT_WHITE);
    return 0;
}
