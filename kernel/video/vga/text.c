/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/



#include <types.h>
#include <console/console.h>
#include <video/video.h>
#include <video/vga/vga.h>

char *vga_text_mem_base = (char *)0xB8000;

void vga_scroll_text(void){
	uint16_t i;
	uint16_t row_size;
	uint16_t screen_size;
	uint16_t last_row_loc;
	
	screen_size = (vga_mode_params[VIDEO_MODE_TEXT].x_width * vga_mode_params[VIDEO_MODE_TEXT].y_height * 2);
	
	row_size = vga_mode_params[VIDEO_MODE_TEXT].x_width * 2;
	
	last_row_loc = screen_size - row_size;
	
	// start at the beginning of the second row (row 1 in zero-based numbering) and copy each value (character and attribute) to the location
	// x_width * 2 positions before it.
	for (i = row_size; i < screen_size; i++){
		vga_text_mem_base[i - row_size] = vga_text_mem_base[i];		
	}
	
	// and now we blank the last line
	for (i = 0; i < row_size; i++){
		vga_text_mem_base[last_row_loc + i] = '\0';
	}
	
	return;
}

uint8_t vga_write_text(const char *txt, uint8_t start_row, uint8_t start_col, uint8_t attrib, video_text_color fg_color, video_text_color bg_color){
	// ignore attrib for now, but I went ahead and put it in the API to minimize breaking things when I add support for it
	
	char *startpoint;
	uint64_t i = 0;	// I mean, it probably doesn't need to be 64 bits, but just to avoid unanticipated issues...
	
	startpoint = (char *)vga_text_mem_base + ((start_row * vga_mode_params[VIDEO_MODE_TEXT].x_width * 2) + (start_col * 2));
	
	while (txt[i]){
		// make sure we don't write past the end of the vga text mode memory area
		if (startpoint >= (vga_text_mem_base + (vga_mode_params[VIDEO_MODE_TEXT].x_width * vga_mode_params[VIDEO_MODE_TEXT].y_height * 2))){
			return 0;
		}
		
		*startpoint = txt[i];
		
		startpoint++;
		
		*startpoint = (bg_color << 4) + fg_color;
		
		startpoint++;
		
		i++;
	}
	
	cursor_set_position((uint16_t)((uint64_t)(startpoint - vga_text_mem_base) / 2));
	
	return 1;
}