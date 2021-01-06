/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/asm/asm.h>
#include <sys/i386/interrupts/irq.h>
#include <sys/i386/interrupts/de.h>
#include <sys/i386/interrupts/idt.h>

#include <sys/debug/assert.h>

#define IDT_SIZE 256

idtEntry idt[IDT_SIZE];

void idt_init(){
	idtr idtr;
	idt_add_ISR(isrDE, DE);
	
	// IRQs
	idt_add_ISR(irq0, IRQ0);
	idt_add_ISR(irq1, IRQ1);
	idt_add_ISR(irq2, IRQ2);
	idt_add_ISR(irq3, IRQ3);
	idt_add_ISR(irq4, IRQ4);
	idt_add_ISR(irq5, IRQ5);
	idt_add_ISR(irq6, IRQ6);
	idt_add_ISR(irq7, IRQ7);
	idt_add_ISR(irq8, IRQ8);
	idt_add_ISR(irq9, IRQ9);
	idt_add_ISR(irq10, IRQ10);
	idt_add_ISR(irq11, IRQ11);
	idt_add_ISR(irq12, IRQ12);
	idt_add_ISR(irq13, IRQ13);
	idt_add_ISR(irq14, IRQ14);
	idt_add_ISR(irq15, IRQ15);
	
	idtr.limit = (IDT_SIZE * sizeof(idtEntry)) - 1;
	idtr.base = (uint64_t)&idt;
	
	asm volatile(
		"lidt %0" :: "m"(idtr)
	    );
	
	//asm_sti()
}

void idt_add_ISR(void *func, intVectors vector){
	ASSERT_NOT_NULL(func, "func cannot be null");
	idt[vector].offsetWordLow = (uint16_t)((uint64_t)func & 0xFFFF);
	idt[vector].selector = 8;
	idt[vector].ist = 0;
	idt[vector].attrs = 0x8E;
	idt[vector].offsetWordMiddleLow = (uint16_t)(((uint64_t)func >> 16) & 0xFFFF);
	idt[vector].offsetDwordHigh = (uint32_t)(((uint64_t)func >> 32) & 0xFFFFFFFF);
	idt[vector].reserved = 0;
}
