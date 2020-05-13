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
	
#ifndef _CONSOLE_C
void initVideoConsole();
void refreshConsole();

extern const char *vidMemBase;
extern volatile char *vidMem;
extern cursorLoc consoleCursorLoc;
extern char consoleBuffer[25][81];
#else
void clearConsoleBuffer();

const char *vidMemBase = (const char *)0xB8000;
volatile char *vidMem;
cursorLoc consoleCursorLoc = {0, 0};

// does not support setting attributes - for now assumed to be 0x0F
// should change this at some point
char consoleBuffer[25][81];
#endif

#ifndef _CONSOLEPRINT_C
void consWriteLine(const char *s);
#else
#endif

#ifndef _CONSOLEUTIL_C
uint8_t findBlankLine();
void scrollConsoleUp();
#else
#endif

#endif