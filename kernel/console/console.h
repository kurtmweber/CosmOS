/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VIDEO_H
#define _VIDEO_H

#include <types.h>

// CRT control I/O ports
#define CRT_INDEX_REGISTER	0x3D4
#define CRT_DATA_REGISTER	0x3D5

// cursor control commands
#define CURSOR_LOCATION_HIBYTE	0x0E
#define CURSOR_LOCATION_LOBYTE	0x0F

#define KBD_PORT		0x60
#define KBD_CTRL_PORT		0x64

typedef struct cursor_loc{
	uint8_t x;
	uint8_t y;
	} cursor_loc;
	
#ifndef _CONSOLE_C
void init_video_console();
void refresh_console();

extern const char *vid_mem_base;
extern volatile char *vid_mem;
extern cursor_loc console_cursor_loc;
extern char console_buffer[25][81];
#else
void clear_console_buffer();

const char *vid_mem_base = (const char *)0xB8000;
volatile char *vid_mem;
cursor_loc console_cursor_loc = {0, 0};

// does not support setting attributes - for now assumed to be 0x0F
// should change this at some point
char console_buffer[25][81];
#endif

#ifndef _CONSOLEPRINT_C
void console_write_line(const char *s);
#else
#endif

#ifndef _CONSOLEUTIL_C
uint8_t find_blank_line();
void scroll_console_up();
#else
#endif

#ifndef _CURSOR_C
extern uint16_t cursor_position;

void cursor_set_position(uint16_t loc);
#else
uint16_t cursor_position;
#endif

#ifndef _KEYBOARD_C
void keyboard_irq_read();
#endif

#endif