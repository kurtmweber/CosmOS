/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PANIC_C
#define _PANIC_C

#include <asm/asm.h>
#include <console/console.h>

void panic(const char *s){
	console_write_line("!!!PANIC!!!");
	console_write_line(s);
	
	asm_hlt();
}

#endif