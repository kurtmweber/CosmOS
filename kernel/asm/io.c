/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _IO_C
#define _IO_C

#include <types.h>

uint8_t asm_in_b(uint16_t port){
	uint8_t data;
	
	asm volatile(
		"in %1, %0"
		:"=a"(data)
		:"Nd"(port)
	    );
	
	return data;
}

uint32_t asm_in_d(uint16_t port){
	uint32_t data;
	
	asm volatile(
		"in %1, %0"
		:"=a"(data)
		:"Nd"(port)
	    );
	
	return data;
}

uint16_t asm_in_w(uint16_t port){
	uint16_t data;
	
	asm volatile(
		"in %1, %0"
		:"=a"(data)
		:"Nd"(port)
	    );
	
	return data;
}

void asm_out_b(uint16_t port, uint8_t data){
	asm volatile(
		"out %1, %0"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}


void asm_out_d(uint16_t port, uint32_t data){
	asm volatile(
		"out %1, %0"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}

void asm_out_w(uint16_t port, uint16_t data){
	asm volatile(
		"out %1, %0"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}

#endif