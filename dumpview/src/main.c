/*
 Title:		Main
 Author:	Badre
 Created:	23/12/2023
*/

#include <defines.h>
#include <stdio.h>
#include <mos_api.h>
#include <agon/vdp_vdu.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "vkey.h"
#include "conio_agon.h"
#include "dumpview.h"
#include "agon_timer.h"


#define DEBUG 1
#define BUFFER_SIZE	65535

int main(int argc, char *argv[])
{			
	char *filename = NULL;
	uint32_t size;		
	uint8_t fh;	
	FIL *fil;

	if  (argc != 2) {
		puts("\r\nUsage: dumpview <filename>\r\n");
		return 1;
	}

	filename = argv[1];
	fh = mos_fopen(filename, FA_READ);
	if (fh == 0) {
		puts("\r\nError to open file or file doesn't exist\r\n");
		return 1;
	}	

	fil = mos_getfil(fh);
	size = fil->obj.objsize;

	if(size > BUFFER_SIZE) {
		puts("\r\nFile is too big!\r\n");
		return 1;
	}

	printf("\r\nLoading file ...\r\n");
	uint32_t sizeArray = size + 1;
    uint8_t * psBuf = (uint8_t *)malloc(sizeArray * sizeof(uint8_t));
	if (psBuf == NULL) {
		printf("\r\nError allocating memory \r\n");
		return 1;
	}  
    memset(psBuf, 0, sizeArray); 
	dumpFileToBuffer(fh, psBuf);
	printf("\r\nFile loaded\r\n");

	initSysvar();		
	if ( vdp_key_init() == -1 ) return 1;
	
	saveScrMmode();  // get mode 
	setmode(scr_mode);	
	clrscr();
	cursorEnable(false);
	
	//volatile uint32_t  *addr = (volatile uint32_t *)0xB0000;	
	dumpviewver((volatile uint8_t *)psBuf, 	sizeArray);
	free(psBuf);
	mos_fclose(fh);	

	mos_i2c_close();
	cursorEnable(true);	
	textattr(BLACK, BRIGHT_WHITE);
	setSavedScrMode();

	return 0;
}

//Dump to buffer
uint32_t dumpFileToBuffer(uint8_t handle, uint8_t * Buffer)
{
  	uint32_t i;
  	i = 0;
 
	while(!mos_feof(handle)) {
		uint8_t ch = mos_fgetc(handle);
		Buffer[i++] = ch;
	}      
  Buffer[i] = '\0';
  return i;
}


