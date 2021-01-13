/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/console/console.h>
#include <sys/console/drivers/drivers.h>
#include <sys/console/drivers/vga_console/vga_console.h>
#include <sys/i386/video/vga/vga.h>
#include <sys/i386/video/video.h>

uint16_t vga_console_x_width;
uint16_t vga_console_y_height;
uint8_t vga_console_xpos;
uint8_t vga_console_ypos;

void vga_console_init() {
    video_query_resolution(&vga_console_x_width, &vga_console_y_height);

    vga_console_xpos = 0;
    vga_console_ypos = 0;

    return;
}

void vga_console_register() {
    console_interfaces[CONSOLE_DRIVER_VGA].init = &vga_console_init;
    console_interfaces[CONSOLE_DRIVER_VGA].setpos = &vga_console_setpos;
    console_interfaces[CONSOLE_DRIVER_VGA].write = &vga_console_write;

    return;
}

uint8_t vga_console_setpos(uint8_t x, uint8_t y) {
    // error if position is out of range
    if ((x >= vga_console_x_width) || (y >= vga_console_y_height)) {
        return 0;
    }

    vga_console_xpos = x;
    vga_console_ypos = y;

    return 1;
}

uint8_t vga_console_write(const char *c) {
    uint64_t i = 0;  // just in case...it's on the stack anyway, so it's a single-digit # of extra bytes that's gone once we return
    char s[2];

    s[1] = '\0';

    while (c[i]) {
        if (c[i] == '\n') {
            vga_console_xpos = 0;
            vga_console_ypos++;
            i++;

            continue;
        } else if (c[i] == '\t') {
            vga_console_xpos += (CONSOLE_TAB_WIDTH - (vga_console_xpos % CONSOLE_TAB_WIDTH));

            if (vga_console_xpos >= vga_console_x_width) {
                vga_console_xpos = 0;
                vga_console_ypos++;
            }

            i++;

            continue;
        }

        if ((vga_console_xpos >= vga_console_x_width)) {  // width is 1-based, pos is 0-based, so if pos = width then we're past the last column
            vga_console_xpos = 0;
            vga_console_ypos++;
        }

        if (vga_console_ypos >= vga_console_y_height) {
            video_scroll_text();
            vga_console_ypos = (vga_console_y_height - 1);  // again, ypos is a 0-based index, while height is a quantity
        }

        s[0] = c[i];

        video_write_text(s, vga_console_ypos, vga_console_xpos, NULL, VIDEO_TEXT_WHITE, VIDEO_TEXT_BLACK);

        vga_console_xpos++;
        i++;
    }

    return 1;
}
