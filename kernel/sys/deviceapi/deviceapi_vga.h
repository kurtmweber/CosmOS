//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all VGA devices will implement
 */
#ifndef _DEVICEAPI_VGA_H
#define _DEVICEAPI_VGA_H

#include <devicemgr/devicemgr.h>
#include <types.h>

enum vga_text_color { VGA_TEXT_BLACK = 0, VGA_TEXT_BLUE, VGA_TEXT_GREEN, VGA_TEXT_CYAN, VGA_TEXT_RED, VGA_TEXT_PURPLE, VGA_TEXT_BROWN, VGA_TEXT_GRAY, VGA_TEXT_DARKGRAY, VGA_TEXT_LIGHTBLUE, VGA_TEXT_LIGHTGREEN, VGA_TEXT_LIGHTCYAN, VGA_TEXT_LIGHTRED, VGA_TEXT_LIGHTPURPLE, VGA_TEXT_YELLOW, VGA_TEXT_WHITE, VGA_TEXT_COLOR_MAX };

enum vga_video_mode { VIDEO_MODE_TEXT = 1, VIDEO_MODE_MAX };

typedef uint8_t (*vga_set_mode_function)(struct device* dev, enum vga_video_mode mode);
typedef void (*vga_scroll_text_function)(struct device* dev);
typedef uint8_t (*vga_write_text_function)(struct device* dev, const char* txt, uint8_t start_row, uint8_t start_col, uint8_t attrib, enum vga_text_color fg_color, enum vga_text_color bg_color);
typedef uint8_t (*vga_query_resolution_function)(struct device* dev, uint16_t* x, uint16_t* y);

struct deviceapi_vga {
    vga_set_mode_function set_mode;
    vga_scroll_text_function scroll_text;
    vga_write_text_function write_text;
    vga_query_resolution_function query_resolution;
};

#endif