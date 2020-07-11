/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _IO_C
#define _IO_C

#include <types.h>

static inline uint8_t asm_in_b(uint16_t port){
	uint8_t data;
	
	asm volatile(
		"in %0, %1"
		:"=a"(data)
		:"Nd"(port)
	    );
	
	return data;
}

static inline uint32_t asm_in_d(uint16_t port){
	uint32_t data;
	
	asm volatile(
		"in %0, %1"
		:"=a"(data)
		:"Nd"(port)
	    );
	
	return data;
}

static inline uint16_t asm_in_w(uint16_t port){
	uint16_t data;
	
	asm volatile(
		"in %0, %1"
		:"=a"(data)
		:"Nd"(port)
	    );
	
	return data;
}

static inline void asm_out_b(uint16_t port, uint8_t data){
	asm volatile(
		"out %0, %1"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}


static inline void asm_out_d(uint16_t port, uint32_t data){
	asm volatile(
		"out %0, %1"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}

static inline void asm_out_w(uint16_t port, uint16_t data){
	asm volatile(
		"out %0, %1"
		:
		: "Nd"(port), "a"(data)
	    );
	
	return;
}

#endif