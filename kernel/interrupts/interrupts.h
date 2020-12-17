/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include <types.h>

// PIC-related constants:

// I/O ports
#define PIC_PRIMARY_COMMAND		0x20
#define PIC_PRIMARY_DATA		0x21
#define PIC_SECONDARY_COMMAND		0xA0
#define PIC_SECONDARY_DATA		0xA1

// IRQ vector offsets for remapping
#define PIC_PRIMARY_VECTOR_OFFSET	0x20
#define PIC_SECONDARY_VECTOR_OFFSET	PIC_PRIMARY_VECTOR_OFFSET + 8

// PIC Mode
#define PIC_MODE_8086			0x01

// Commands
#define PIC_INIT		0x11
#define PIC_EOI			0x20

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
	DE = 0,
	IRQ0 = PIC_PRIMARY_VECTOR_OFFSET,
	IRQ1,
	IRQ2,
	IRQ3,
	IRQ4,
	IRQ5,
	IRQ6,
	IRQ7,
	IRQ8 = PIC_SECONDARY_VECTOR_OFFSET,
	IRQ9,
	IRQ10,
	IRQ11,
	IRQ12,
	IRQ13,
	IRQ14,
	IRQ15
	} intVectors;
	

void idt_init();
void addISR(void *func, intVectors vector);
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

void pic_init();
void pic_send_eoi(uint8_t irq);


#endif