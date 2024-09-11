/*
 Title:		Main
 Author:	Badre
 Created:	14/01/2023
 Last Updated: 01/08/2024

 Modinfo:
 01/08/2024   Modify code from editorKey
 01/08/2024   Add function cpmReadKey that replace editorReadKey
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <mos_api.h>
#include "global.h"
#include "agconio.h"
#include "ckey.h"
#include "uart.h"


#define MAX_X 80
#define MAX_Y 25
#define tohex(x) ((x) < 10 ? (x) + 48 : (x) + 87)

unsigned char mask8bit = 0xff;
const char *getForeAnsiColor(Color c);
const char *getBackAnsiColor(Color c);

void xprintf(const char *format, ...)
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
		_cputs(buf);
	}
	va_end(ap);
}

int setvdpmode(int mode)
{
	_cputch(22);
	_cputch(mode);
	return mode;
}

void _putcon(uint8_t ch) // Puts a character
{
	uint8_t c = utf8ascii2(ch);
	_cputch(c & mask8bit);	

}

void _cputs(const char *str)
{
  while (*str)
  {
    //_putcon(*(str++));
    _cputch(*(str++));
  }
}

void _cputch(uint8_t ch)
{
  /*if (SERCTL_ENABLED)
  {
    SEROUT.write(ch);
  }*/    
	
  #if REDIRECT_UART
    uart1_putch(ch);
  #else
    printf("%c", ch);
  #endif

#ifdef FABGL
  Terminal.write(ch);
#endif
}

void _cputchutf8(uint8_t ch)
{
  uint8_t c = utf8ascii2(ch);

  /*if (SERCTL_ENABLED)
  {
    SEROUT.write(c & mask8bit);
  }*/
  //putchar(ch);
	//printf("%c", c);
  _cputch(c);

#ifdef FABGL
  Terminal.write(c & mask8bit);
#endif
}

void _putsmsg(int x, int y, const char *s)
{
  _gotoxy(x, y);
  _cputs(s);
  _cputch('\r');
  _cputch('\n');
}

void _cputsxy(int x, int y, const char *s)
{
  _gotoxy(x, y);
  while (*s)
  {
    _cputch(*s++);
  }
}

void _blank(int width)
{
  if(width < 1 ) return;    
  for(int i = 0; i < width; i++) 
    _cputch(' ');  
}

uint8_t _chready(void) // Checks if there's a character ready for input
{
  return (_kbhit() ? 0xff : 0x00);
}

uint8_t _getchNB(void) // Gets a character, non-blocking, no echo
{
  return (_kbhit() ? _getch() : 0x00);
}

void _bell(void)
{
  _cputch(7);
}

void _clearInputBuffer()
{
  while (_kbhit()) {
    _readKbd();  
  } 
  
}

void _termFlush()
{
#ifdef FABGL
  Terminal.flush();
#endif
	uart1_flush();
}

int _kbhit(void)
{
  /*if (SERCTL_ENABLED)
  {
    return (SEROUT.available());
  }*/

  return (int)uart1_kbhit();
#ifdef FABGL
  return (Terminal.available());
#endif
}

int _readKbd(void)
{
 /* if (SERCTL_ENABLED)
  {
    return (SEROUT.read());
  }*/
  return uart1_getch();

#ifdef FABGL
  return (Terminal.read());
#endif
}

uint8_t _getche(void)
{
  uint8_t ch = _getch();
  _cputch(ch);
  return (ch);
}

uint8_t _getch(void)
{	
	while (!_kbhit())
	;
	return _readKbd();
}

void _puthex8(uint8_t c) // Puts a HH hex string
{
  _cputch(tohex(c >> 4));
  _cputch(tohex(c & 0x0f));
}

void _puthex16(uint16_t w) // puts a HHHH hex string
{
  _puthex8(w >> 8);
  _puthex8(w & 0x00ff);
}

void _clearline(char x, char y)
{
  _gotoxy(x, y);
  _clrline();
}

void _clrline(void)
{
  _cputs("\033[2K");
}

void _clrlinetocursor(void)
{
  _cputs("\033[1K");
}

void _clrcursortoendline(void)
{
  _cputs("\033[0K");
}

void _clrcursortoendscreen(void)
{
  _cputs("\033[0J");
}

void _clearlines(int x, int y, int height)
{
	int i = 0;
	for (i = 0; i < height; i++)
	{
	_gotoxy(x, y + i);
	_clreol();
	}
}

void _clreol(void)
{
  _cputs("\033[K");
}

void _clrscr()
{
  _cputs("\033[H\033[J");
}

void _cursoroff(void)
{
  _cputs("\033[?25l");
}

void _cursoron(void)
{
  _cputs("\033[?25h");
}

void _cursorbar(void)
{
  _cputs("\033[5 q");
}

void _cursorunderline(void)
{
  _cputs("\033[3 q");
}

void _cursorblock(void)
{
  _cputs("\033[0 q");
}

void _cursortype(CursorType ct)
{
  xprintf("\033[%d q", ct);  
}

void _savescreen(void)
{
  _cputs("\033[?47h");
}

void _restorescreen(void)
{
  _cputs("\033[?47l");
}

void _gotoxy(char x, char y)
{
  /*if (x > Terminal.getColumns())
    x = Terminal.getColumns();
  else if (x < 1) x = 1;

  if (y > Terminal.getRows())
    y = Terminal.getRows();
  else if (y < 1) y = 1;*/

  if (x > MAX_X)
    x = MAX_X;
  else if (x < 1)
    x = 1;

  if (y > MAX_Y)
    y = MAX_Y;
  else if (y < 1)
    y = 1;

  xprintf("%c[%d;%df",0x1B,y,x);
  /*_cputch('\033');
  _cputch('[');
  _cputch((y / 10) + '0');
  _cputch((y % 10) + '0'); 
  _cputch(';');
  _cputch((x / 10) + '0');
  _cputch((x % 10) + '0');
  _cputch('f');*/
}

void _termcolor(void)
{
  _textattr(Black, Green);
}

void _textcolor(Color tc)
{
  _cputs(getForeAnsiColor(tc));
}

void _textattr(Color bg, Color fg)
{
  _textcolor(fg);
  _textbackground(bg);
}

void _textbackground(Color c)
{
  _cputs(getBackAnsiColor(c));
}

void waitKey(void)
{
  _cputs("\r\nTry any key to continue ...");
  //while (!_kbhit())
   // ;
   _getch();
}

const char *getForeAnsiColor(Color c)
{
  switch (c)
  {
  case Black:
    return ("\033[30m");
    break;
  case Red:
    return ("\033[31m");
    break;
  case Green:
    return ("\033[32m");
    break;
  case Yellow:
    return ("\033[33m");
    break;
  case Blue:
    return ("\033[34m");
    break;
  case Magenta:
    return ("\033[35m");
    break;
  case Cyan:
    return ("\033[36m");
    break;
  case White:
    return ("\033[37m");
    break;
  case BrightBlack:
    return ("\033[90m");
    break;
  case BrightRed:
    return ("\033[91m");
    break;
  case BrightGreen:
    return ("\033[92m");
    break;
  case BrightYellow:
    return ("\033[93m");
    break;
  case BrightBlue:
    return ("\033[94m");
    break;
  case BrightMagenta:
    return ("\033[95m");
    break;
  case BrightCyan:
    return ("\033[96m");
    break;
  case BrightWhite:
    return ("\033[97m");
    break;
  case Lightgray:
    return ("\033[38;2;211;211;211m");  //ESC[38;2;{r};{g};{b}m
    break;
  case Orange:
    return ("\033[38;2;265;165;0m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkYellow:
    return ("\033[38;2;204;204;0m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkBlue:
    return ("\033[38;2;0;0;139m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkRed:
    return ("\033[38;2;139;0;0m");  //ESC[38;2;{r};{g};{b}m
    break;
  case DarkGrey:
    return ("\033[38;2;169;169;169m");  //ESC[38;2;{r};{g};{b}m
    break;
  default:
    return ("\033[92m");
    break;
  }
}

const char *getBackAnsiColor(Color c)
{
  switch (c)
  {
  case Black:
    return ("\033[40m");
    break;
  case Red:
    return ("\033[41m");
    break;
  case Green:
    return ("\033[42m");
    break;
  case Yellow:
    return ("\033[43m");
    break;
  case Blue:
    return ("\033[44m");
    break;
  case Magenta:
    return ("\033[45m");
    break;
  case Cyan:
    return ("\033[46m");
    break;
  case White:
    return ("\033[47m");
    break;
  case BrightBlack:
    return ("\033[100m");
    break;
  case BrightRed:
    return ("\033[101m");
    break;
  case BrightGreen:
    return ("\033[102m");
    break;
  case BrightYellow:
    return ("\033[103m");
    break;
  case BrightBlue:
    return ("\033[104m");
    break;
  case BrightMagenta:
    return ("\033[105m");    
    break;
  case BrightCyan:
    return ("\033[106m");
    break;
  case BrightWhite:
    return ("\033[107m");
    break;
  case Lightgray:
    return ("\033[48;2;211;211;211m"); //ESC[48;2;{r};{g};{b}m
    break;
  case Orange:
    return ("\033[48;2;265;165;0m"); 
    break;
  case DarkYellow:
    return ("\033[48;2;204;204;0m");  
    break;
  case DarkBlue:
    return ("\033[48;2;0;0;139m");  
    break;
  case DarkRed:
    return ("\033[48;2;139;0;0m");  
    break;
  case DarkGrey:
    return ("\033[48;2;169;169;169m");  
    break;
  default:
    return ("\033[92m");
    break;
  }
}

void _moveCursor(Curs curs, int col)
{
  if (curs == RIGHT)
  {
    _cursorfwd((char)col);
  }
  else if (curs == LEFT)
  {
    _cursorback((char)col);
  }
  else
    return;
}

void _moveCursorCol(int col)
{
  xprintf("\033[%dG", col);
}

void _saveCursor()
{
  _cputs("\033[s");
}

void _restoreCursor()
{
  _cputs("\033[u");
}

void _cursorHome()
{
  _cputch('\r');
}

void _cursorfwd(int n)
{
  xprintf("\033[%dC", n);
}

void _cursorback(int n)
{
  xprintf("\033[%dD", n);
}

void _cursorright(void)
{
  _cputs("\033[C");
}

void _cursorleft(void)
{
  _cputs("\033[D");
}

void _scroll_region(char top, char bottom)
{
  _cputch('\033');
  _cputch('[');
  _cputch(top);
  _cputch(';');
  _cputch(bottom);
  _cputch('r');
}

void _scroll_all(void)
{
  _cputch('\033');
  _cputch('[');
  _cputch('r');
}

void _clrbottom(void)
{
  _cputch('\033');
  _cputch('[');
  _cputch('0');
  _cputch('J');
}

void _clrtop(void)
{
  _cputch('\033');
  _cputch('[');
  _cputch('1');
  _cputch('J');
}

void _setmode(int mode)
{
  _cputch('\033');
  _cputch('[');
  _cputch('=');

  if (mode & 0x7)
    _cputch(mode + '0');
  else
  {
    _cputch('1');
    _cputch(((mode & 0x1f) % 10) + '0');
  }
  _cputch('h');
}

void _screenmode(int mode)
{
  _cputch('\033');
  _cputch('[');
  _cputch('=');
  _cputch(mode);
  _cputch('h');
}

void _resetmode(int mode)
{
  _cputch('\033');
  _cputch('[');
  _cputch('=');

  if (mode & 0x7)
    _cputch(mode + '0');
  else
  {
    _cputch('1');
    _cputch(((mode & 0x1f) % 10) + '0');
  }
  _cputch('l');
}

bool waitMsg(char x, char y, const char *msg)
{
  bool ret = false;
  int ch;
  //_crlf();
  _gotoxy(x, y);
  _cputs(msg);
  _crlf();
  _termFlush();
  ch = _getch();
  if (ch == CTRL_KEY('c'))
    ret = true;
  return ret;
}

bool _pause(const char *msg)
{
  bool ret = false;
  int ch;
   
  _cputs(msg);
  _crlf();
  _termFlush();
  ch =  _getch();
  if (ch == CTRL_KEY('c'))
    ret = true;
  return ret; 

  /*_cputs("\r\nTry any key to continue ...");
  while (!_kbhit())
    ;*/
}

// Return
void _crlf(void)
{
  //_cputch('\r');
  //_cputch('\n');
  _cputs("\r\n");
}

void _resetblink(void)
{
  _cputs("\033[25m");
}

void _resetunderline(void)
{
  _cputs("\033[24m");
}

void _resetitalic(void)
{
  _cputs("\033[23m");
}

void _resetbold(void)
{
  _cputs("\033[22m");
}

void _resetstrikethrough(void)
{
  _cputs("\033[29m");
}

void _resetinverse(void)
{
  _cputs("\033[27m");
}

void _setcolors(int foreground, int background, int attribute)
{
  xprintf("\033[%i;%i;%im", attribute, foreground, background);  
}

void _blink(void)
{
  _cputs("\033[5m");
}

void _underline(void)
{
  _cputs("\033[4m");
}

void _italic(void)
{
  _cputs("\033[3m");
}

void _bold(void)
{
  _cputs("\033[1m");
}

void _strikethrough(void)
{
  _cputs("\033[9m");
}

void _inverse(void)
{
  _cputs("\033[7m");
}

void _resetall(void)
{
  _cputs("\033[0m");
}

void _scrollscreen(void)
{
  _cputs("\033[r");
}

void _scrolldown(void)
{
  _cputs("\033[D");
}

void _scrollup(void)
{
  _cputs("\033[M");
}

void _scroll(int deb, int fin)
{
  xprintf("\033[%d;%d", deb, fin);
}

int editorReadKey()
{
  return cpmReadKey();

  /*
  uint8_t c;  
    
  c = _readKbd(); 
 
  if (c == '\033')
  {
    char seq[6];

    seq[0] = _readKbd();   
    seq[1] = _readKbd();
    if (seq[0] == '[')
    {
      if (seq[1] >= '0' && seq[1] <= '9')
      {
        seq[2] = _readKbd();
        if (seq[2] == '~')
        {
          switch (seq[1])
          {
          case '1':
            return HOME_KEY;
          case '2':
            return INS_KEY;
          case '3':
            return DEL_KEY;
          case '4':
            return END_KEY;
          case '5':
            return PAGE_UP;
          case '6':
            return PAGE_DOWN;
          case '7':
            return HOME_KEY;
          case '8':
            return END_KEY;
          }
        }
        else
        { // F5 to F8
          seq[3] = _readKbd();
          if (seq[3] == '~' && seq[1] == '1')
          {
            switch (seq[2])
            {
            case '5':
              return F5_KEY;
            case '7':
              return F6_KEY;
            case '8':
              return F7_KEY;
            case '9':
              return F8_KEY;
            }
          }
          else if (seq[3] == '~' && seq[1] == '2')
          { // F9 to F12
            switch (seq[2])
            {
            case '0':
              return F9_KEY;
            case '1':
              return F10_KEY;
            case '3':
              return F11_KEY;
            case '4':
              return F12_KEY;
            }
          }
        }
      }
      else
      {
        switch (seq[1])
        {
        case 'A':
          return ARROW_UP;
        case 'B':
          return ARROW_DOWN;
        case 'C':
          return ARROW_RIGHT;
        case 'D':
          return ARROW_LEFT;
        case 'H':
          return HOME_KEY;
        case 'F':
          return END_KEY;
        }
      }
    }
    else if (seq[0] == 'O')
    { // F1 to F4
      switch (seq[1])
      {
      case 'P':
        return F1_KEY;
      case 'Q':
        return F2_KEY;
      case 'R':
        return F3_KEY;
      case 'S':
        return F4_KEY;
      }
    }
    else if (seq[0] == '0')
    {
      switch (seq[1])
      {
      case 'H':
        return HOME_KEY;
      case 'F':
        return END_KEY;
      }
    }

    return '\033';
  }
  else
  {
    return c;
  }  */
}

// ****** UTF8-Decoder: convert UTF8-string to extended ASCII *******
static uint8_t c1; // Last character buffer

// Convert a single Character from UTF8 to Extended ASCII
// Return "0" if a byte has to be ignored
uint8_t utf8ascii2(uint8_t ascii)
{
	uint8_t last;
	
  if (ascii < 128) // Standard ASCII-set 0..0x7F handling
  {
    c1 = 0;
    return (ascii);
  }

  // get previous input
  last = c1; // get last char
  c1 = ascii;     // remember actual character

  switch (last) // conversion depending on first UTF8-character
  {
  case 0xC2:
    return (ascii);
    break;
  case 0xC3:
    return (ascii | 0xC0);
    break;
  case 0x82:
    if (ascii == 0xAC)
      return (0x80); // special case Euro-symbol
  }

  return (0); // otherwise: return zero, if character has to be ignored
}

// In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
void utf8ascii(uint8_t *s, size_t len)
{
	int k = 0;
	size_t i;
	uint8_t c;
	for (i = 0; i < len; i++)
	{
		c = utf8ascii2(s[i]);
		if (c != 0)
			s[k++] = c;
	}
	s[k] = 0;
}

void _centermsg(int y, const char *msg, Color bg, Color fg)
{
  _textattr(bg, fg);
  _gotoxy(1, y);
  _clrline();
  _gotoxy((MAX_X - (strlen(msg))) / 2, y);
  _cputs(msg);
  _clreol();
}
