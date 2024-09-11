/*
 Title:		keytest.c
 Author:	Badre
 Created:	25/07/2024
*/

#include <stdio.h>
#include <mos_api.h>
#include "agconio.h"
#include "uart.h"

#define TIMEOUT 1000
#define END 0

extern void	uart1_handler(void);

static unsigned char t_getch(int timeout) {
  char ch = END;
  int t = timeout;
  while (t && !_kbhit()) {
    t--;
  }
  if (_kbhit()) {
    ch = _getch();
  }
  return ch;
}

void describe(unsigned char c) {
  if (!c) {
    printf("<nul> ");
  } else if (c == 27) {
    printf("<esc> ");
  } else if (c == 13) {
    printf("<cr> ");
  } else if (c == 10) {
    printf("<lf> ");
  } else if (c == 9) {
    printf("<tab> ");
  } else if (c == 32) {
    printf("<spc> ");
  } else if (c<32) {
    printf("^%c ", c+64);
  } else if (c>127) {
    printf("<%d> ", c);
  } else {
    printf("%c ", c);
  }
}

int main(void) {
  unsigned char i, c;
  uint24_t baudrate = 115200;		
	void *oldvector;
	xUART 	pUART;

  pUART.baudRate = baudrate;
	pUART.dataBits = 8;
	pUART.stopBits = 1;
	pUART.parity = PAR_NOPARITY;
	pUART.hwFlowControl = 0;
	pUART.interrupts = UART_IER_RECEIVEINT;
		
	oldvector = mos_setintvector(UART1_IVECT, uart1_handler);

	init_UART1();
	open_UART1(&pUART);

  _termFlush();
  unsigned char c1[10];
  printf("Press keys or ^C to exit:\n");

  while (1) {
    i = 0;
    c1[i++] = _getch();
    do {
        c = t_getch(TIMEOUT);
        c1[i++] = c;
    } while (c != END);
    printf("You pressed: ");
    for (i=0; c1[i]!=END; i++) {
      describe(c1[i]);
    }
    printf("\n");
    if(c1[0] == 3) {
      printf("Exiting...\n");
      break;
    }
  }
  close_UART1();
	mos_setintvector(UART1_IVECT, oldvector);

  return 0;
}