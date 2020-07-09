/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

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

void asm_out_b(uint16_t port, uint8_t data){
	asm volatile(
		"outb %0, %1"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}

void asm_out_w(uint16_t port, uint16_t data){
	asm volatile(
		"outw %0, %1"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}

void asm_sti(){
	asm volatile(
		"sti"
	    );
	
	return;
}