/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLE_C
#define _CONSOLE_C

#include <console/console.h>

void init_video_console(){
	clear_console_buffer();
	
	return;
}
	
void clear_console_buffer(){
	uint8_t i, j;
	
	for (i = 0; i < 25; i++){
		for (j = 0; j < 81; j++){
			console_buffer[i][j] = 0;
		}
	}
	
	return;
}

void refresh_console(){
	uint8_t i, j;
	
	vid_mem = (volatile char *)vid_mem_base;
	
	for (i = 0; i < 25; i++){
		for (j = 0; j < 80; j++){	// the 81st character in the buffer line (index 80) is not displayed
			*vid_mem++ = console_buffer[i][j];
			*vid_mem++ = 0x0F;
		}
	}

}

#endif