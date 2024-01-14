/*
 Title:	Main
 Author:	Badre
 Created:	08/01/2024
*/

#include <stdio.h>
#include <mos_api.h>
#include <stdint.h>
#include <stdbool.h>
#include <agon_timer.h>
#include "uart.h"


#define FILE_BUFFERSIZE 	16384

extern void	uart1_handler(void);

void handle_uart1(uint24_t baudrate, char *fname);

int main(int argc, char * argv[])
{		
	uint24_t baudrate = 115200;	
	char *filename = NULL;

	if  (argc != 2) {
		puts("\r\nUsage: send <filename>\r\n");
		return 0;
	}	
	
	filename = argv[1];
	
	handle_uart1(baudrate, filename);	
	return 0;
}

void handle_uart1(uint24_t baudrate, char *fname) 
{	
	void *oldvector;
	xUART 	pUART;		
    uint32_t size;		
	uint8_t fh;	
	FIL *fil;
	
	fh = mos_fopen(fname, FA_READ);
	if (fh == 0) {
		puts("\r\nError to open file or file doesn't exist\r\n");
		return;
	}	
	
	fil = mos_getfil(fh);
	size = fil->obj.objsize;
	
	pUART.baudRate = baudrate;
	pUART.dataBits = 8;
	pUART.stopBits = 1;
	pUART.parity = 0;
	pUART.hwFlowControl = 0;
	pUART.interrupts = UART_IER_RECEIVEINT;

	oldvector = mos_setintvector(UART1_IVECT, uart1_handler);
	
	init_UART1();
	open_UART1(&pUART);	
		
	uart1_flush();	
	delayms(10);
	
	if(fh)
	{		
		printf("\r\nTransfert file %s\r\n", fname);
		while(!mos_feof(fh))
		{
			uart1_putch(mos_fgetc(fh));			
		}
		
		printf("\r\nTransfert successfull of %lu Byte(s)\r\n", size);
	} else printf("\r\nCouldn't open %s", fname);
	
	mos_fclose(fh);				
	close_UART1();
	mos_setintvector(UART1_IVECT, oldvector);	
}





