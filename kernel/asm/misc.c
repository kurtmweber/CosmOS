/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ASM_C
#define _ASM_C

#include <types.h>
#include <mm/mm.h>

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
		"mov %0, cr3"
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