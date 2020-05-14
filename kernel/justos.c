/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <asm/asm.h>
#include <console/console.h>
#include <interrupts/interrupts.h>
#include <string/string.h>
#include <types.h>

extern char _end[];

void JustOS(){
	char testString[UINT64_DECIMAL_STRING_LENGTH];
	
	initVideoConsole();
	consWriteLine("JustOS");
	consWriteLine("Initializing IDT");
	initIDT();
	
	
	asm_hlt();
}