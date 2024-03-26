/*
 Title:		font.h
 Author:	Badre
 Created:	24/03/2024
*/
#include <stdint.h>
#include <stdbool.h>

#ifndef _FONT_H
#define _FONT_H

#define ROW_COUNT(array)    (sizeof(array) / sizeof(*array))

typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	int len;			//Len array font / 8
	uint8_t first_char;
	uint8_t last_char;
	const uint8_t *data; /*!< Pointer to data font data array */
} FontDef_t;

void load_font(FontDef_t fontdef);
void load_range_font(FontDef_t fontdef);
bool redefine_num_font(uint8_t *array, uint8_t num);
void set_default_font();

//extern uint8_t console_font_8x8[];
extern FontDef_t Font_cp437_8x8;
extern FontDef_t Font_box_8x8;
extern FontDef_t Font_ext_latin_8x8;
extern FontDef_t Font_c64_8x8;

#endif