/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLE_C
#define _CONSOLE_C

#include <console/console.h>

void initVideoConsole(){
	clearConsoleBuffer();
	
	return;
}
	
void clearConsoleBuffer(){
	uint8_t i, j;
	
	for (i = 0; i < 25; i++){
		for (j = 0; j < 81; j++){
			consoleBuffer[i][j] = 0;
		}
	}
	
	return;
}

void refreshConsole(){
	uint8_t i, j;
	
	vidMem = (volatile char *)vidMemBase;
	
	for (i = 0; i < 25; i++){
		for (j = 0; j < 80; j++){	// the 81st character in the buffer line (index 80) is not displayed
			*vidMem++ = consoleBuffer[i][j];
			*vidMem++ = 0x0F;
		}
	}

}

#endif