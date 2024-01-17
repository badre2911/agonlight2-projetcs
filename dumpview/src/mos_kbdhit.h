/*
 Title:		mos_kbdhit.h
 Author:	Badre
 Created:	23/12/2023 
 Last Updated: 17/01/2024

 Modinfo:
 17/01/2024   Added kbd_code and read_key
 
*/

#ifndef _KEYHIT_H
#define _KEYHIT_H

#include <defines.h>

extern char kbd_hit(void);
extern uint8_t kbd_ascii(void);
extern uint8_t kbd_code(void);
extern char read_key(void);

#endif
