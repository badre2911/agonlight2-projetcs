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
#define BUFFER_SIZE	204800 


int main(int argc, char *argv[])
{			
	char *filename = NULL;
	long size;			
	FILE *f;

	if  (argc != 2) {
		puts("\r\nUsage: dumpview <filename>\r\n");
		return 1;
	}

	filename = argv[1];
	f = fopen(filename, "rb");
	if (f == NULL) {
		puts("\r\nError to open file or file doesn't exist\r\n");
		return -1;
	}
	fseek(f, 0L, SEEK_END);
	size = ftell(f);	

	if(size > (long)BUFFER_SIZE) {
		puts("\r\nFile is too big!\r\n");
		fclose(f);
		return -1;
	}
	fseek(f, 0L, SEEK_SET);
	printf("\r\nLoading file ... Size %lu Byte(s)\r\n", size);

	long sizeArray = size + 1;
    uint8_t * psBuf = (uint8_t *)malloc(sizeArray * sizeof(uint8_t));
	if (psBuf == NULL) {
		printf("\r\nError allocating memory \r\n");
		fclose(f);
		return -1;
	}  
	
	fread(psBuf, sizeArray, 1, f);
	/*if(1 != fread(psBuf, sizeArray, 1, f)) {
		printf("Cannot read blocks in file\r\n");
		fclose(f);
		return -1;
	}*/

	printf("\r\nFile loaded\r\n");

	initSysvar();		
	if ( vdp_key_init() == -1 ) return 1;
	
	saveScrMmode();  // get mode 
	setmode(scr_mode);	
	clrscr();
	cursorEnable(false);
		
	dumpviewver((uint8_t *)psBuf, 	(unsigned long)sizeArray);
	free(psBuf);		
	fclose(f);

	cursorEnable(true);	
	textattr(BLACK, BRIGHT_WHITE);
	setSavedScrMode();

	return 0;
}


