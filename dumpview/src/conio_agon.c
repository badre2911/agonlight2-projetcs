/*
 Title:		conio_agon.c
 Author:	Badre
 Created:	23/12/2023
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <mos_api.h>
#include <agon/vdp_vdu.h>
#include "conio_agon.h"
#include "agon_timer.h"



#define LINE_SPACES	"                                                                                "

static volatile SYSVAR *sv;

//void rtc_unpack(uint8_t * buffer, struct tm * t);
//void rtc_formatDateTime(char * buffer, struct tm * t);

void clearRect(int x1, int y1, int x2, int y2, int color)
{		
	if(!sv) sv = vdp_vdu_init();	
	graphicColor(scr_mode, color);

	vdp_logical_scr_dims( false );
	vdp_get_scr_dims( true );

	x1 *= (sv->scrWidth / sv->scrCols);
	y1 *= (sv->scrHeight / sv->scrRows);
	x2 *= (sv->scrWidth / sv->scrCols);
	y2 *= (sv->scrHeight / sv->scrRows);		

    putch(25);
    putch(4);
	putch(x1 & 0xFF);
    putch(x1 >> 8);
    putch(y1 & 0xFF);
    putch(y1 >> 8);

	putch(25);
    putch(101);
	putch(x2 & 0xFF);
    putch(x2 >> 8);
    putch(y2 & 0xFF);
    putch(y2 >> 8);

}

void cursorEnable(bool flag)
{
    vdp_cursor_enable(flag);
}

void gotoxy(int row, int col)
{	
    vdp_cursor_tab(row, col);		
}

void textattr(int bg, int fg)
{
	vdp_set_text_colour(fg);
    vdp_set_text_colour(bg+128);
}

void textcolor(int fg)
{
    vdp_set_text_colour(fg);	
}

void graphicColor(int mode, int color)
{
	putch(18),
	putch(mode);
	putch(color);
}

void textbackground(unsigned char bg)
{
	vdp_set_text_colour(bg+128);	
}

void crlf()
{
	printf("\r\n");
}
 
/*void clreol()
{	
	uint8_t x_cursor;
	uint8_t y_cursor;	
	uint8_t i;
	
	if(!sv) sv = vdp_vdu_init();

	x_cursor =  vdp_cursorGetXpos();  	 
	y_cursor =  vdp_cursorGetYpos(); // > 0) ? vdp_cursorGetYpos() - 1 : 0 ;	
	gotoxy(y_cursor, x_cursor);
	
	for (i = 0; i < sv->scrCols - x_cursor; i++)	
		printf("%c", ' ');
	//printf("\r\nxcur: %d  ycur: %d 80-xcur: %d\r\n", x_cursor, y_cursor, 80-x_cursor);
}*/

void clearlines(int col, int row, int height)
{
	for (int i = 0; i < height; i++)
	{
		gotoxy(row + i, col );		
		clreol();			
	}	
}

void clrscr()
{
    vdp_clear_screen();
}

void statusBar(int col, int row, const char *text) 
{
	gotoxy(row, col);
	for (size_t i = 0; i < strlen(text); i++) {
		printf("%c", text[i]);
	}
	
}

void xprintf(int row, int col, const char *format, ...)
{
	int size;
	va_list ap;
	va_start(ap, format);

	size = vsnprintf(NULL, 0, format, ap) + 1;
	if (size > 0)
	{
		va_end(ap);
		va_start(ap, format);
		char buf[size + 1];
		vsnprintf(buf, size, format, ap);
		gotoxy(row, col);
		printf("%s", buf);
		clreol();
	}
	va_end(ap);
}

int setmode(int mode)
{
	putch(22);
	putch(mode);
	return mode;
}

void initSysvar()
{
	sv = vdp_vdu_init();
}

uint8_t vdp_cursorGetXpos(void)
{	
	if(!sv) sv = vdp_vdu_init();
	
	sv->vpd_pflags = 0x00;	
	putch(23);	// VDP command
	putch(0);	// VDP command
	putch(0x82);	// Request cursor position
	while(!(sv->vpd_pflags & vdp_pflag_cursor));

	return(sv->cursorX);
}

uint8_t vdp_cursorGetYpos(void)
{	
	if(!sv) sv = vdp_vdu_init();
	
	sv->vpd_pflags = 0x00;	
	putch(23);	// VDP command
	putch(0);	// VDP command
	putch(0x82);	// Request cursor position
	while(!(sv->vpd_pflags & vdp_pflag_cursor));

	return(sv->cursorY);
}

