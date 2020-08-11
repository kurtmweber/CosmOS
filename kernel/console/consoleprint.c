/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLEPRINT_C
#define _CONSOLEPRINT_C

#include <types.h>
#include <console/console.h>

void console_write_line(const char *s){
	uint8_t line_no;
	uint8_t i = 0;
	
	line_no = find_blank_line();
	
	// if we're writing on screen line 25 (array index 24, which is what is returned by findBlankLine())
	// then we need to scroll the screen up a line
	if (line_no == 25){
		scroll_console_up();
		line_no--;
	}
	
	while(s[i]){
		if (i == 80){
			break;
		}
		
		console_buffer[line_no][i] = s[i];
		i++;
	}
	
	//cursor_position = (line_no * 80) + i;
	
	refresh_console();

	return;
}

#endif