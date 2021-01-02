/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <system/system_platform/i386/mm/mm.h>

#ifdef TARGET_PLATFORM_I386

void asm_hlt(){
	asm volatile(
		"hlt"
	    );
	
	return;
}

void asm_cli(){
	asm volatile(
		"cli"
	    );
	
	return;
}

pttentry asm_cr3_read(){
	pttentry cr3;
	
	asm volatile(
		"mov %%cr3 ,%0"
		: "=r" (cr3)
	    );
	
	return cr3;
}

void asm_sti(){
	asm volatile(
		"sti"
	    );
	
	return;
}

#endif