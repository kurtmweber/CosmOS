/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include <types.h>
#include <dev/pic/pic.h>
#include <interrupts/idt.h>


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
	
void isrDE(stackFrame *frame);

void irq0(stackFrame *frame);
void irq1(stackFrame *frame);
void irq2(stackFrame *frame);
void irq3(stackFrame *frame);
void irq4(stackFrame *frame);
void irq5(stackFrame *frame);
void irq6(stackFrame *frame);
void irq7(stackFrame *frame);
void irq8(stackFrame *frame);
void irq9(stackFrame *frame);
void irq10(stackFrame *frame);
void irq11(stackFrame *frame);
void irq12(stackFrame *frame);
void irq13(stackFrame *frame);
void irq14(stackFrame *frame);
void irq15(stackFrame *frame);

#endif