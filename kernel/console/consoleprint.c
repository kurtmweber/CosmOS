/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLEPRINT_C
#define _CONSOLEPRINT_C

#include <types.h>
#include <console/console.h>

void consWriteLine(const char *s){
	uint8_t lineNo;
	uint8_t i = 0;
	
	lineNo = findBlankLine();
	
	// if we're writing on screen line 25 (array index 24, which is what is returned by findBlankLine())
	// then we need to scroll the screen up a line
	if (lineNo == 25){
		scrollConsoleUp();
		lineNo--;
	}
	
	while(s[i]){
		if (i == 80){
			break;
		}
		
		consoleBuffer[lineNo][i] = s[i];
		i++;
	}
	
	refreshConsole();

	return;
}

#endif