/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/i386/video/vga/vga.h>
#include <sys/i386/video/video.h>
#include <types.h>

void video_scroll_text(void) {
    video_interfaces[video_active_driver].scroll_text();
    return;
}

uint8_t video_write_text(const char *txt, uint8_t start_row, uint8_t start_col, uint8_t attrib, video_text_color fg_color, video_text_color bg_color) {
    return video_interfaces[video_active_driver].write_text(txt, start_row, start_col, attrib, fg_color, bg_color);
}
