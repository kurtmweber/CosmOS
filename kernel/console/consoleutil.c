/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLEUTIL_C
#define _CONSOLEUTIL_C

#include <types.h>
#include <console/console.h>

uint8_t findBlankLine(){
	// return the 0-based array index of the first line in consoleBuffer that is empty
	// an empty line is one that is all NULL values
	// if no lines are empty, return 24 (row 25 on screen); caller will use this to bump off the top line
	// and scroll the rest of the screen up before printing
	
	uint8_t i, j;
	bool inUse = false;
	
	for (i = 0; i < 25; i++){
		inUse = false;
		for (j = 0; j < 81; j++){
			if (consoleBuffer[i][j] != 0){
				inUse = true;
			}
		}
		if (!inUse){
			return i;
		}
	}
	
	// if all lines are in use, return 25
	// 25 is not actually a usable index, so the caller will have to test for it
	// e.g. to see whether scrolling is needed
	return 25;
}

void scrollConsoleUp(){
	uint8_t i, j;
	
	// don't do this on the last line!
	for (i = 0; i < 24; i++){
		for (j = 0; j < 81; j++){
			consoleBuffer[i][j] = consoleBuffer[i+1][j];
		}
	}
	
	for (j = 0; j < 81; j++){
		consoleBuffer[24][j] = 0;
	}
	
	return;
}

#endif