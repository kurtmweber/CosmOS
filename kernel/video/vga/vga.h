/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VGA_H
#define _VGA_H

#include <types.h>
#include <video/video.h>

#define VGA_DRIVER_ENABLED

// CRT control I/O ports
#define CRT_INDEX_REGISTER	0x3D4
#define CRT_DATA_REGISTER	0x3D5

// cursor control commands
#define CURSOR_LOCATION_HIBYTE	0x0E
#define CURSOR_LOCATION_LOBYTE	0x0F

typedef enum vga_text_color{
	VGA_TEXT_BLACK = 0,
	VGA_TEXT_BLUE,
	VGA_TEXT_GREEN,
	VGA_TEXT_CYAN,
	VGA_TEXT_RED,
	VGA_TEXT_PURPLE,
	VGA_TEXT_BROWN,
	VGA_TEXT_GRAY,
	VGA_TEXT_DARKGRAY,
	VGA_TEXT_LIGHTBLUE,
	VGA_TEXT_LIGHTGREEN,
	VGA_TEXT_LIGHTCYAN,
	VGA_TEXT_LIGHTRED,
	VGA_TEXT_LIGHTPURPLE,
	VGA_TEXT_YELLOW,
	VGA_TEXT_WHITE,
	VGA_TEXT_COLOR_MAX
} vga_text_color;

typedef struct vga_mode_params_t{
	// x-width and y-height are either characters for text modes, or pixels for graphics modes
	uint16_t x_width;
	uint16_t y_height;
} vga_mode_params_t;


#ifndef _CURSOR_C
extern uint16_t cursor_position;

void cursor_set_position(uint16_t loc);
#else
uint16_t cursor_position;
#endif

#ifndef _MODE_C
uint8_t vga_set_mode(video_mode mode);
#endif

#ifndef _VGA_TEXT_C
void vga_scroll_text(void);
uint8_t vga_write_text(const char *txt, uint8_t start_row, uint8_t start_col, uint8_t attrib, video_text_color fg_color, video_text_color bg_color);
#else
char *vga_text_mem_base = (char *)0xB8000;
#endif

#ifndef _VGA_C
extern vga_mode_params_t vga_mode_params[VIDEO_MODE_MAX];

void vga_driver_register();
#else
vga_mode_params_t vga_mode_params[VIDEO_MODE_MAX];

void vga_init();
uint8_t vga_query_resolution(uint16_t *x, uint16_t *y);
#endif

#endif