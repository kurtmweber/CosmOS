/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <debug_error/debug_error.h>
#include <interrupts/interrupts.h>
#include <keyboard/keyboard.h>
#include <mm/mm.h>
#include <string/string.h>
#include <timing/timing.h>

#define P(row, col)	((row << 5) + col)

void CosmOS(){
	key_action_t k;
	void *p, *q, *r, *s, *t, *u;
	
	init_video_console();
	
	console_write_line("CosmOS");
	
	console_write_line("Initializing IDT");
	initIDT();
	
	console_write_line("Initializing MMU");
	mmu_init();
	
	console_write_line("Initializing PIC");
	pic_init();
	
	keyboard_init();
	
	asm_sti();
	
	while (1){
		asm_hlt();
	}
}

#undef P