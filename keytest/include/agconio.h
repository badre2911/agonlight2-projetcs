/*
 Title:		Main
 Author:	Badre
 Created:	14/01/2023
*/

#ifndef _CONIO_H
#define _CONIO_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "global.h"
#include "vdpmode.h"

/*typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long   DWORD;
typedef uint8           byte;*/

#define VT 11

#define CTRL_KEY(k) ((k) & 0x1f)
#define ALT(k)      ((k) + (161 - 'a')) //not work
#define CTRL_ALT(k) ((k) + (129 - 'a')) //not work

extern void saveScrMmode(void);
extern void setSavedScrMode(void);

static int scr_mode = VDPMODE_640x480_16C; 

bool waitMsg(char x, char y, const char * msg);
void waitKey();
void xprintf(const char * format, ...);
int setvdpmode(int mode);

void _bell(void);
void _clearInputBuffer();
void _termFlush();
void _putcon(uint8_t ch);
void _cputch(uint8_t c);
void _putchutf8(uint8_t ch);
void _cputs(const char *str);
void _putsmsg(int x, int y, const char *s);
void _cputsxy(int, int, const char *);
void _blank(int width);
void _puthex8(uint8_t c);
void _puthex16(uint16_t c);
int _kbhit(void);
int _readKbd();
void _clrscr();
void _clreol(void);
void _clearline(char x, char y);
void _clrline(void);
void _clrlinetocursor(void);
void _clrcursortoendline(void);
void _clrcursortoendscreen(void);
void _clearlines(int x, int y, int height);
void _cursoroff(void);
void _cursoron(void);
void _cursorbar(void);
void _cursorunderline(void);
void _cursorblock(void);
void _cursortype(CursorType ct);
void _savescreen(void);
void _restorescreen(void);
void _gotoxy(char x, char y);
void _textbackground(Color c);
void _textattr(Color bg, Color fg);
void _textcolor(Color tc);
void _termcolor(void);
uint8_t _getch(void);
uint8_t _getche(void);
uint8_t _chready(void);
uint8_t _getchNB(void);
void _moveCursor(Curs curs, int col);
void _moveCursorCol(int col);
void _saveCursor();
void _restoreCursor();
void _cursorHome();
void _cursorfwd(int n);
void _cursorback(int n);
void _cursorright(void);
void _cursorleft(void);
void _scroll_region(char top, char bottom);
void _scroll_all(void);
void _clrbottom(void);
void _clrtop(void);
void _setmode(int mode);
void _screenmode(int mode);
void _resetmode(int mode);
bool _pause(const char *msg);
void _crlf(void);
void _resetblink(void);
void _resetunderline(void);
void _resetitalic(void);
void _resetbold(void);
void _resetstrikethrough(void);
void _resetinverse(void);
void _setcolors(int foreground, int background, int attribute);
void _blink(void);
void _underline(void);
void _italic(void);
void _bold(void);
void _strikethrough(void);
void _resetall(void);
void _scrollscreen(void);
void _scrolldown(void);
void _scrollup(void);
void _scroll(int deb, int fin);
void _centermsg(int y, const char * msg, Color bg, Color fg);

int editorReadKey();
void utf8ascii(uint8_t *s, size_t len);
uint8_t utf8ascii2(uint8_t ascii);


#endif
