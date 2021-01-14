/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/i386/video/vga/vga.h>
#include <sys/i386/video/video.h>
#include <types.h>

uint8_t vga_set_mode(video_mode mode) {
    if (mode == VIDEO_MODE_TEXT) {
        return 1;
    } else {
        return 0;
    }
}
