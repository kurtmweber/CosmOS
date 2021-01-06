//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _IDT_H
#define _IDT_H

#include <types.h>
#include <dev/i386/pic/pic.h>


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
	

void idt_init();
void idt_add_ISR(void *func, intVectors vector);

#endif


