//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _IDT_H
#define _IDT_H

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
void idt_add_ISR(void *func, intVectors vector);

#endif


