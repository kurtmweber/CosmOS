/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <debug_error/debug_error.h>
#include <interrupts/interrupts.h>
#include <mm/mm.h>
#include <string/string.h>

void JustOS(){
	void *p, *q, *r, *s, *t, *u;
	
	init_video_console();
	
	console_write_line("CosmOS");
	
	console_write_line("Initializing IDT");
	initIDT();
	
	console_write_line("Initializing MMU");
	mmu_init();
	
	asm_hlt();
}