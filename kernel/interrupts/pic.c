/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <asm/asm.h>
#include <interrupts/interrupts.h>

void pic_init(){
	// send init command to prim/sec PICs
	asm_out_b(PIC_PRIMARY_COMMAND, PIC_INIT);
	asm_out_b(PIC_SECONDARY_COMMAND, PIC_INIT);
	
	// remap IRQ vectors
	asm_out_b(PIC_PRIMARY_DATA, PIC_PRIMARY_VECTOR_OFFSET);
	asm_out_b(PIC_SECONDARY_DATA, PIC_SECONDARY_VECTOR_OFFSET);
	
	// let PICs know how they are connected to one another
	asm_out_b(PIC_PRIMARY_DATA, 0x04);	// set bit 2 (0-based) to inform primary PIC that a secondary PIC is connected at IRQ2
	asm_out_b(PIC_SECONDARY_DATA, 0x02);	// for secondary, we use the value (rather than bit position) to let it know that it's connected to
						// IRQ2 on the primary
	
	// and then set the PICs to 8086 mode
	asm_out_b(PIC_PRIMARY_DATA, PIC_MODE_8086);
	asm_out_b(PIC_SECONDARY_DATA, PIC_MODE_8086);
	
	return;
}

void pic_send_eoi(uint8_t irq){
	asm_out_b(PIC_PRIMARY_COMMAND, PIC_EOI);
	
	if ((irq + PIC_PRIMARY_VECTOR_OFFSET) >= PIC_SECONDARY_VECTOR_OFFSET){
		asm_out_b(PIC_SECONDARY_COMMAND, PIC_EOI);
	}
}
