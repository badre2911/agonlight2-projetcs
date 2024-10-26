/*
 Title:		mos_kbdhit.h
 Author:	Badre
 Created:	23/12/2023 
 Last Updated: 26/10/2024

 Modinfo:
 17/01/2024		replace getsysvar_vkeycode by kbd_code
 26/10/2024		Change F10 key with F7 key for Quit
*/

#include <stdio.h>
#include <stdint.h>
#include <agon/vdp_vdu.h>
#include "dumpview.h"
#include "mos_kbdhit.h"
#include "vkey.h"
#include "conio_agon.h"

#define VT 11

#define TOP_MARGIN 3
#define BYTE_COLUMN 16

const char * rtc_days[7][2] = {	
	{ "Sun", "Sunday" },
	{ "Mon", "Monday" },
	{ "Tue", "Tuesday" },
	{ "Wed", "Wednesday" },
	{ "Thu", "Thursday" },
	{ "Fri", "Friday" },
	{ "Sat", "Saturday" },
};

const char * rtc_months[12][2] = {
	{ "Jan", "January" },
	{ "Feb", "February" },
	{ "Mar", "March" },
	{ "Apr", "April" },
	{ "May", "May" },
	{ "Jun", "June" },
	{ "Jul", "July" },
	{ "Aug", "August" },
	{ "Sep", "September" },
	{ "Oct", "October" },
	{ "Nov", "November" },
	{ "Dec", "December" },
};


static volatile SYSVAR *sv;

const char pgUp[] = "Page Up";
const char pgDn[] = "Page Down";
const char ArrowUp[] = "Key Up";
const char ArrowDn[] = "Key Down";
const char KeyHome[] = "Key Home";
const char keyEnd[] = "Key End";
const char keyF10[] = "Quit";
const char * const dumpview_fkeys[] = {ArrowUp, ArrowDn, pgDn, pgUp, KeyHome, keyEnd, keyF10, 0, 0, 0};

const static char hexchars[] = "0123456789ABCDEF";
static const uint8_t *debut = NULL;
static const uint8_t *fin = NULL;

static size_t ligneDebut;
static size_t nombreDeLignes;

const size_t hauteurEcran = 20;
const char hexdumpTitle[] = "Dumpview";

const char pgmStatus[] = "ARROW UP    ARROW DOWN    PGDN    PGUP    HOME    END  F10 Quit";


void printTitle();
int printDate( void );
static const uint8_t *pointeurDebutLigne(size_t numeroLigne);
static void imprime(size_t numeroLigne);


static const uint8_t *pointeurDebutLigne(size_t numeroLigne)
{
	size_t i;
	const uint8_t *ptr = debut;
	for (i = 0; i <= numeroLigne; i++)
	{
		ptr = &debut[i * BYTE_COLUMN];
	}
	return ptr;
}

size_t i;
static void imprime(size_t numeroLigne)
{
	//char hexVal[9] = {'\0'};
	uint8_t *mptr = NULL;
	uint8_t *ptr = (uint8_t *)pointeurDebutLigne(numeroLigne);
	mptr = ptr;
   
	for (i = 0; i < hauteurEcran && (&ptr[i * BYTE_COLUMN] < fin); i++)
	{
		uint8_t amount = 0;
		uint8_t j = 8;
		uint8_t nbRow = 0;
		int spaces;
		  
		mptr = &ptr[i * BYTE_COLUMN];
		gotoxy(i + TOP_MARGIN, LEFT_MARGIN);		
		//clearRect(LEFT_MARGIN, TOP_MARGIN + i, MAX_X, TOP_MARGIN + i, BLACK); 

		nbRow = ((fin - mptr) < BYTE_COLUMN) ? (fin - mptr) - 1 : BYTE_COLUMN;
		amount = BYTE_COLUMN - nbRow;
		textattr(BLACK, BRIGHT_YELLOW);
		
		do
		{
			j--;			
			printf("%c", hexchars[((uint32_t)(mptr - debut) >> (j * 4)) & 0xF]);
		} while (j > 0);
		
		printf(": ");
		textattr(BLACK, BRIGHT_WHITE);

		// Data
		for (j = 0; j < nbRow; j++)
		{			
			printf("%02X ", *mptr);
			mptr++;
		}

		//int spaces;
		// Fill with spaces in case we couldn't fill an entire row (due to reaching the end of the file)
		for (spaces = amount * 3; spaces; spaces--)
		  printf(" ");
		  
		printf(" ");		
        printf("|");
		printf(" ");
		
		textattr(BLACK, BRIGHT_GREEN);
		// ASCII
		mptr = &ptr[i * BYTE_COLUMN];
		for (j = 0; j < nbRow; j++)
		{
		  if (*mptr == VT)
			printf(".");					   
		  else if (*mptr >= ' ' && *mptr <= '~')			
		    printf("%c", *mptr);
		  else				  		
		    putch('.');
		  mptr++;
		}
		
		clreol();	
	}
	
	//Clear rest of lines
	size_t rest = nombreDeLignes-ligneDebut;
	if(rest < hauteurEcran) {
		int rest = nombreDeLignes-ligneDebut;
		int clearZone_y1 = TOP_MARGIN + rest;
		int clearZone_y2 = clearZone_y1 + (hauteurEcran - rest);
		clearRect(LEFT_MARGIN, clearZone_y1, MAX_X, clearZone_y2, BLACK); 
	}
}

void dumpviewver(void *start, unsigned long sizearray)
{
	VKey vkey;	
	bool saisie = false;
	
	debut = (const uint8_t *)start;
	fin = debut + sizearray;
	ligneDebut = 0;		
	nombreDeLignes = (sizearray / BYTE_COLUMN) + (sizearray % BYTE_COLUMN ? 1 : 0);
		
	if(!sv) sv = vdp_vdu_init();	
	clrscr();
	cursorEnable(false);	
	printTitle();
	drawfkeys(LEFT_MARGIN, ROW_FKEY, dumpview_fkeys);
	
	imprime(ligneDebut);

	while (saisie == false)
	{		
		if(kbd_hit())
		{				
			vkey = kbd_code();  
			
			if (vkey == VK_UP || vkey == VK_KP_UP)
			{
				if (ligneDebut > 0)
				{
					ligneDebut -= 1;	
					printTitle();
					imprime(ligneDebut);				
				} 				
				
			}
			else if (vkey == VK_PAGEUP || vkey == VK_KP_PAGEUP)
			{
				if (ligneDebut >= hauteurEcran)
				{
					ligneDebut -= hauteurEcran;								
				}				
				else
					ligneDebut = 0;
				printTitle();
				imprime(ligneDebut);
			}
			else if (vkey == VK_DOWN || vkey == VK_KP_DOWN)
			{	
				if(nombreDeLignes <= hauteurEcran) continue;				
				if ((ligneDebut < (nombreDeLignes - hauteurEcran)) && ((nombreDeLignes - hauteurEcran) > hauteurEcran))
				{
					ligneDebut += 1;
					printTitle();
					imprime(ligneDebut);
				} 
			}
			else if (vkey == VK_PAGEDOWN || vkey == VK_KP_PAGEDOWN)
			{			
				if(nombreDeLignes <= hauteurEcran) continue;
				
				if ((ligneDebut + hauteurEcran) < nombreDeLignes)
				{					
					ligneDebut += hauteurEcran;	
					printTitle();					
					imprime(ligneDebut);
				}								
				
			}
			else if (vkey == VK_HOME || vkey == VK_KP_HOME)
			{
				ligneDebut = 0;
				printTitle();
				imprime(ligneDebut);
			}
			else if (vkey == VK_END || vkey == VK_KP_END)
			{
				if (nombreDeLignes > hauteurEcran)
				{
				ligneDebut = nombreDeLignes - hauteurEcran;
				printTitle();
				imprime(ligneDebut);
				}
			}
			else if (vkey == VK_F7)
			{
				saisie = true;
			}	
						
			sv->vkeydown = 0;			
		}		
	}
	crlf();
	debut = NULL;
	fin = NULL;

	return;
}


void printTitle()
{
	textattr(DARK_GREEN, BRIGHT_WHITE);
	gotoxy(ROW_TITLE, LEFT_MARGIN);
	printf("%s - First: 0x%06x  Last: 0x%06x  NbLig:%05d  Line:%05d", hexdumpTitle, (unsigned int)debut, (unsigned int)fin, (int)nombreDeLignes, ligneDebut);

	textattr(BLACK, DARK_WHITE);
	clreol();
}

void drawfkeys(int row, int col, const char * const fkeys[])
{
	int i, j;

	gotoxy(col, row);
	for (i = 0, j = 0; i < 9; i++)
	{
		if (fkeys[i] == 0) continue;
		textcolor(BRIGHT_WHITE);
		textbackground(BLACK);
		if (j == 9) {
			printf("%c", '1');
			printf("%c", '0');
		} else {
			printf("%c", j + '1');			
		}
		textcolor(BLACK);
		textbackground(DARK_CYAN);
		if (fkeys[i] != 0)
			printf("%s", (char *)fkeys[i]);
		else
			printf("      ");
		textcolor(BRIGHT_WHITE);
		textbackground(BLACK);
		printf(" ");
		
		j++;
	}
}

int printDate( void )
{
	static char rtc_buffer[64];	
	volatile RTC_DATA *rtc = getsysvar_rtc();
	RTC_DATA rtc_cur, rtc_prev;
	
	if(!sv) sv = vdp_vdu_init();	
	rtc_prev = *rtc;
	for ( int i = 0; i < 30; i++ ) 	
	{			
		int len = mos_getrtc( rtc_buffer );
		rtc_buffer[len] = '\0';				
		rtc_cur = *rtc;
		
		if ( rtc_cur.rtc_data != rtc_prev.rtc_data ) {	
			statusBar(LEFT_MARGIN, ROW_STATUS, rtc_buffer);								
			rtc_prev = rtc_cur;
		}		
		for ( int j = 0; j < 10; j++ ) waitvblank();			
	}
	return 0;
}

