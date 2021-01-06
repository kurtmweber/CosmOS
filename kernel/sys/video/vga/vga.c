/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/video/video.h>
#include <sys/video/vga/vga.h>

vga_mode_params_t vga_mode_params[VIDEO_MODE_MAX];

void vga_driver_register(){
	video_interfaces[VIDEO_DRIVER_VGA].init = &vga_init;
	video_interfaces[VIDEO_DRIVER_VGA].query_resolution = &vga_query_resolution;
	video_interfaces[VIDEO_DRIVER_VGA].scroll_text = &vga_scroll_text;
	video_interfaces[VIDEO_DRIVER_VGA].set_mode = &vga_set_mode;
	video_interfaces[VIDEO_DRIVER_VGA].write_text = &vga_write_text;
	
	return;
}

void vga_init(){
	vga_mode_params[VIDEO_MODE_TEXT].x_width = 80;
	vga_mode_params[VIDEO_MODE_TEXT].y_height = 25;
	
	return;
}

uint8_t vga_query_resolution(uint16_t *x, uint16_t *y){
	*x = vga_mode_params[video_active_mode].x_width;
	*y = vga_mode_params[video_active_mode].y_height;
	return 1;
}
