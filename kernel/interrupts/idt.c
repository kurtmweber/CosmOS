/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <interrupts/interrupts.h>

idtEntry idt[256];

void idt_init(){
	idtr idtr;
	addISR(isrDE, DE);
	
	// IRQs
	addISR(irq0, IRQ0);
	addISR(irq1, IRQ1);
	addISR(irq2, IRQ2);
	addISR(irq3, IRQ3);
	addISR(irq4, IRQ4);
	addISR(irq5, IRQ5);
	addISR(irq6, IRQ6);
	addISR(irq7, IRQ7);
	addISR(irq8, IRQ8);
	addISR(irq9, IRQ9);
	addISR(irq10, IRQ10);
	addISR(irq11, IRQ11);
	addISR(irq12, IRQ12);
	addISR(irq13, IRQ13);
	addISR(irq14, IRQ14);
	addISR(irq15, IRQ15);
	
	idtr.limit = (256 * sizeof(idtEntry)) - 1;
	idtr.base = (uint64_t)&idt;
	
	asm volatile(
		"lidt %0" :: "m"(idtr)
	    );
	
	//asm_sti()
}

void addISR(void *func, intVectors vector){
	idt[vector].offsetWordLow = (uint16_t)((uint64_t)func & 0xFFFF);
	idt[vector].selector = 8;
	idt[vector].ist = 0;
	idt[vector].attrs = 0x8E;
	idt[vector].offsetWordMiddleLow = (uint16_t)(((uint64_t)func >> 16) & 0xFFFF);
	idt[vector].offsetDwordHigh = (uint32_t)(((uint64_t)func >> 32) & 0xFFFFFFFF);
	idt[vector].reserved = 0;
}
