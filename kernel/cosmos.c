/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <console/console.h>
#include <console/drivers/drivers.h>
#include <asm/asm.h>
#include <debug_error/debug_error.h>
#include <interrupts/interrupts.h>
#include <keyboard/keyboard.h>
#include <mm/mm.h>
#include <string/string.h>
#include <timing/timing.h>
#include <video/video.h>
#include <video/vga/vga.h>

#define P(row, col)	((row << 5) + col)

void CosmOS(){	
	video_init();
	video_select_driver(VIDEO_DRIVER_VGA);
	video_select_mode(VIDEO_MODE_TEXT);
	
	console_driver_interface_init();
	
	console_write("Loading CosmOS 0.1\n");
	
	console_write("Initializing IDT...\n");
	initIDT();
	
	console_write("Initializing MMU...\n");
	mmu_init();
	
	console_write("Initializing PIC...\n");
	pic_init();
	
	keyboard_init();
	
	asm_sti();
	
	while (1){
		asm_hlt();
	}
}

#undef P