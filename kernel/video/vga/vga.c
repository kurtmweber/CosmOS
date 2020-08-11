/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VGA_C
#define _VGA_C

#include <types.h>
#include <video/video.h>
#include <video/vga/vga.h>

void vga_driver_register(){
	video_interfaces[VIDEO_DRIVER_VGA].init = &vga_init;
	video_interfaces[VIDEO_DRIVER_VGA].set_mode = &vga_set_mode;
	video_interfaces[VIDEO_DRIVER_VGA].write_text = &vga_write_text;
	
	return;
}

void vga_init(){
	vga_mode_params[VIDEO_MODE_TEXT].x_width = 80;
	vga_mode_params[VIDEO_MODE_TEXT].y_height = 25;
	
	return;
}

#endif