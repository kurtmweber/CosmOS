/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VIDEO_H
#define _VIDEO_H

#include <types.h>

typedef struct cursorLoc{
	uint8_t x;
	uint8_t y;
	} cursorLoc;

#ifndef _PRINTK_C
void printk(const char *s);
#else
#endif
	
#ifndef _VIDEO_C
extern volatile char *vidMem;
extern cursorLoc consoleCursorLoc;

void initVideoConsole();
#else
volatile char *vidMem = (volatile char *)0xB8000;
cursorLoc consoleCursorLoc = {0, 0};
#endif

#endif