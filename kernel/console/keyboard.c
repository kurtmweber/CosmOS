/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KEYBOARD_C
#define _KEYBOARD_C

#include <asm/asm.h>
#include <console/console.h>

void keyboard_irq_read(){
	asm_in_b(KBD_PORT);
	
	return;
}

#endif

