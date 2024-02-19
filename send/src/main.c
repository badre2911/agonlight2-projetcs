/*
 Title:		   main.c
 Author:	   Badre
 Created:	   08/01/2024
 Last Update:  19/02/2024

 Modinfo: 19/02/2024 Add message before launch transfert
					 Add possibility to cancel transfert
*/
#include <stdio.h>
#include <mos_api.h>
#include <stdint.h>
#include <stdbool.h>
#include <agon_timer.h>
#include <agon\vdp_key.h>
#include <agon\vdp_vdu.h>
#include "uart.h"
#include "mos_kbdhit.h"
#include "vkey.h"


#define FILE_BUFFERSIZE 	131072

extern void	uart1_handler(void);
static volatile SYSVAR *sv;

bool waitEscMsg(const char *msg);
void handle_uart1(uint24_t baudrate, char *fname);


int main(int argc, char * argv[])
{		
	uint24_t baudrate = 115200;	
	char *filename = NULL;

	if(!sv) sv = vdp_vdu_init();
	if ( vdp_key_init() == -1 ) return -1;

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
	uint24_t nbOctets = 0;
	bool cancel = false;		

	uart1_flush();
	vdp_clear_screen();
	if(waitEscMsg("Press any key for continue or ESC for cancel\r\n")) {
		return;
	}

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
			VKey vkey = 0;
			uart1_putch(mos_fgetc(fh));	
			if ((nbOctets++ % 500) == 0) putch('.');
			if(kbd_hit() && (vkey != VK_DOWN))
			{								
				vkey = sv->vkeycode;					
				if(vkey == VK_ESCAPE) {
					cancel = true;
					break;
				}			
				sv->vkeydown = 0;			
			}			
		}
		
		if(cancel) {
			printf("\r\nTransfert canceled by user.\r\n");
		} else {
			printf("\r\nTransfert successfull of %lu Byte(s)\r\n", size);
		}
		
	} else printf("\r\nCouldn't open %s", fname);
	
	mos_fclose(fh);				
	close_UART1();
	mos_setintvector(UART1_IVECT, oldvector);	
}

bool waitEscMsg(const char *msg) {
	VKey vkey;
	bool saisie = false;
	
	if(!sv) sv = vdp_vdu_init();
	printf("%s\r\n", (const char *)msg);		 
	while(saisie == false)
	{
		while(!kbd_hit());

		vkey = kbd_code(); 
		if(vkey == VK_ESCAPE)
		{			
			saisie = true;
			break;
			
		} 		
		sv->vkeydown = 0;	
		break;									
	}
	return saisie;	
}



