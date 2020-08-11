/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VIDEO_C
#define _VIDEO_C

#include <types.h>
#include <video/video.h>
#include <video/vga/vga.h>

void video_init(){
	uint8_t i;
	
#ifdef VGA_DRIVER_ENABLED
	vga_driver_register();
#endif
	for (i = 0; i < VIDEO_DRIVER_LAST; i++){
		video_interfaces[i].init();
	}
}

uint8_t video_select_driver(video_driver driver){
	if (driver >= VIDEO_DRIVER_LAST){
		return 0;
	}
	
	video_active_driver = driver;
	return 1;
}

uint8_t video_select_mode(video_mode mode){
	if (mode >= VIDEO_MODE_MAX){
		return 0;
	}
	
	if (!video_interfaces[video_active_driver].set_mode(mode)){
		return 0;
	}
	
	video_active_mode = mode;
	return 1;
}

#endif