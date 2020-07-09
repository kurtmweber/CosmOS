/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLEUTIL_C
#define _CONSOLEUTIL_C

#include <types.h>
#include <console/console.h>

uint8_t find_blank_line(){
	// return the 0-based array index of the first line in consoleBuffer that is empty
	// an empty line is one that is all NULL values
	// if no lines are empty, return 24 (row 25 on screen); caller will use this to bump off the top line
	// and scroll the rest of the screen up before printing
	
	uint8_t i, j;
	bool in_use = false;
	
	for (i = 0; i < 25; i++){
		in_use = false;
		for (j = 0; j < 81; j++){
			if (console_buffer[i][j] != 0){
				in_use = true;
			}
		}
		if (!in_use){
			return i;
		}
	}
	
	// if all lines are in use, return 25
	// 25 is not actually a usable index, so the caller will have to test for it
	// e.g. to see whether scrolling is needed
	return 25;
}

void scroll_console_up(){
	uint8_t i, j;
	
	// don't do this on the last line!
	for (i = 0; i < 24; i++){
		for (j = 0; j < 81; j++){
			console_buffer[i][j] = console_buffer[i+1][j];
		}
	}
	
	for (j = 0; j < 81; j++){
		console_buffer[24][j] = 0;
	}
	
	return;
}

#endif