/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VGA_CONSOLE_C
#define _VGA_CONSOLE_C

#include <console/drivers/drivers.h>
#include <console/drivers/vga/vga.h>
#include <video/video.h>
#include <video/vga/vga.h>

void vga_console_init(){
	video_query_resolution(&vga_console_x_width, &vga_console_y_height);
	
	vga_console_xpos = 0;
	vga_console_ypos = 0;
	
	return;
}

void vga_console_register(){
	console_interfaces[CONSOLE_DRIVER_VGA].init = &vga_console_init;
	console_interfaces[CONSOLE_DRIVER_VGA].setpos = &vga_console_setpos;
	console_interfaces[CONSOLE_DRIVER_VGA].write = &vga_console_write;
	
	return;
}

uint8_t vga_console_setpos(uint8_t x, uint8_t y){
	// error if position is out of range
	if ((x >= vga_console_x_width) || (y >= vga_console_y_height)){
		return 0;
	}
	
	vga_console_xpos = x;
	vga_console_ypos = y;
	
	return 1;
}

#endif