
/*
 Title:		mos_kbdhit.h
 Author:	Badre
 Created:	23/12/2023 
 Last Updated: 17/01/2024

 Modinfo:
 17/01/2024   Update code for waitMsg
 
*/

#include <stdio.h>
#include <mos_api.h>
#include <stdint.h>
#include <stdbool.h>
#include "conio_agon.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

static void printAscii(int ascii);
static void printHead();

int main()
{			
	int thisByte = 32;
  	int lig = 0;

	clrscr();
	cursorEnable(false);
	printHead();
			
	while (true)
	{
		if (thisByte > 254) break;
		printAscii(thisByte);
			
		lig++;
		thisByte++;		
		if ((lig % 50) == 0 && lig != 0) {
			if(waitMsg("Press any key for continue or CTRL+c for quit ...\r\n")) {
				cursorEnable(true);
				return 0;
			}
		}
	}
	//Display rest of line ascii
	if((thisByte % 254) != 0)
	{			
		for (int i = 0; i < thisByte % 254; i++)
		{
			printAscii(thisByte);      
		}					
	}
	
	waitMsg("Press any key for quit.");
	cursorEnable(true);
	return 0;
}

static void printHead() {
	clrscr();
	gotoxy(1, 1);
	printf("ASCII Table ~ Character Map\r\n\r\n");
}

static void printAscii(int ascii)
{
	if(ascii == 127 ) {
		printf("DEL"); //Delete key
		printf("\tDEC: ");
	}
	else {
		printf("%c", ascii);
		printf("\t\t\tDEC: ");
	}	  	
		
  	printf("%d", ascii);	
	if(ascii < 100) 
  		printf(",  HEX: ");
	else
		printf(", HEX: ");
	printf("%02X", ascii);
	printf(", OCT: ");
	printf("%o", ascii);
	printf(", BIN: ");
	printf("%c%c%c%c%c%c%c%c", BYTE_TO_BINARY(ascii));
	printf("\r\n");
}



