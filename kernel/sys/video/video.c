/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/video/video.h>
#include <sys/video/vga/vga.h>

video_driver_interface_t video_interfaces[VIDEO_DRIVER_LAST];
video_driver video_active_driver;
video_mode video_active_mode;

void video_init(){
	uint8_t i;
	
#ifdef VGA_DRIVER_ENABLED
	vga_driver_register();
#endif
	for (i = 0; i < VIDEO_DRIVER_LAST; i++){
		video_interfaces[i].init();
	}
}

uint8_t video_query_resolution(uint16_t *x, uint16_t *y){
	uint8_t r;
	return video_interfaces[video_active_driver].query_resolution(x, y);
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
