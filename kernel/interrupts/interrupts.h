/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include <types.h>

typedef struct idtEntry{
	uint16_t offsetWordLow;		// offset bits 0-15
	uint16_t selector;		// per bootloader, should be 0x08
	uint8_t ist;			// always zero, we don't use ist
	uint8_t attrs;			// bits 0-3: gate type (should always be 1110)
					// bit 4 = 0
					// bit 5-6 = min. privilege level (should be 00)
					// bit 7 = present (should be 1 if there is a handler installed, otherise 0)
	uint16_t offsetWordMiddleLow;	// offset bits 16-31
	uint32_t offsetDwordHigh;	// offset bits 32-63
	uint32_t reserved;		// 0
	
	}__attribute__((packed)) idtEntry;
	
typedef struct idtr{
	uint16_t limit;
	uint64_t base;
	}__attribute__((packed)) idtr;
	
typedef struct stackFrame{
	uint64_t rip;
	uint64_t cs;
	uint64_t fflags;
	uint64_t rsp;
	uint64_t ss;
	}__attribute__((packed)) stackFrame;
	
typedef enum intVectors{
	DE = 0
	} intVectors;
	
#ifndef _IDT_C
void initIDT();
#else
idtEntry idt[256];

void addISR(void *func, intVectors vector);
void isrDE(stackFrame *frame);
#endif

#endif