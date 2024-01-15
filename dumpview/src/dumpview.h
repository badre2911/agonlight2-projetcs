/*
 Title:		dumpview.h
 Author:	Badre
 Created:	23/12/2023
*/

#ifndef _DUMPVIEW_H
#define _DUMPVIEW_H

void dumpviewver(volatile void * start, unsigned long sizearray);
uint32_t dumpFileToBuffer(uint8_t handle, uint8_t * Buffer);

#endif
