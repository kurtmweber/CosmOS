/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VGA_CONSOLE_WRITE_C
#define _VGA_CONSOLE_WRITE_C

#include <types.h>
#include <console/drivers/vga/vga.h>
#include <video/video.h>

uint8_t vga_console_write(const char *c){
	uint64_t i = 0;	// just in case...it's on the stack anyway, so it's a single-digit # of extra bytes that's gone once we return
	char s[2];
	
	s[1] = '\0';
	
	while (c[i]){
		if (c[i] == '\n'){
			vga_console_xpos = 0;
			vga_console_ypos++;
			i++;
			
			continue;
		}
		
		if ((vga_console_xpos >= vga_console_x_width)){	// width is 1-based, pos is 0-based, so if pos = width then we're past the last column
			vga_console_xpos = 0;
			vga_console_ypos++;
		}
		
		if (vga_console_ypos >= vga_console_y_height){
			video_scroll_text();
			vga_console_ypos = (vga_console_y_height - 1);	// again, ypos is a 0-based index, while height is a quantity
		}
		
		s[0] = c[i];
		
		video_write_text(s, vga_console_ypos, vga_console_xpos, NULL, VIDEO_TEXT_WHITE, VIDEO_TEXT_BLACK);
		
		vga_console_xpos++;
		i++;
	}
	
	return 1;
}

#endif