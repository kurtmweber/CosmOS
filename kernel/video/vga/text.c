/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VGA_TEXT_C
#define _VGA_TEXT_C

#include <types.h>
#include <video/video.h>
#include <video/vga/vga.h>

uint8_t vga_write_text(const char *txt, uint8_t start_row, uint8_t start_col, uint8_t attrib, video_text_color fg_color, video_text_color bg_color){
	// ignore attrib for now, but I went ahead and put it in the API to minimize breaking things when I add support for it
	
	char *startpoint;
	uint64_t i = 0;	// I mean, it probably doesn't need to be 64 bits, but just to avoid unanticipated issues...
	
	startpoint = (char *)vga_text_mem_base + ((start_row * vga_mode_params[VIDEO_MODE_TEXT].x_width * 2) + (start_col * 2));
	
	while (txt[i]){
		*startpoint = txt[i];
		
		startpoint++;
		
		*startpoint = (bg_color << 4) + fg_color;
		
		startpoint++;
		
		i++;
	}
	
	cursor_set_position((uint16_t)((uint64_t)(startpoint - (char *)vga_text_mem_base) / 2));
}

#endif