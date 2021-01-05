/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/pic/pic.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <debug/assert.h>

// I/O ports
#define PIC_PRIMARY_COMMAND		0x20
#define PIC_PRIMARY_DATA		0x21
#define PIC_SECONDARY_COMMAND	0xA0
#define PIC_SECONDARY_DATA		0xA1

// PIC Mode
#define PIC_MODE_8086			0x01

// Commands
#define PIC_INIT		0x11
#define PIC_EOI			0x20

/*
* perform device instance specific init here
*/
void pic_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s (%s)\n" ,dev->description, dev->name);

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
}

void pic_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "8259 PIC");
	deviceinstance->devicetype = PIC;
	deviceinstance->init =  &pic_device_init;
	devicemgr_register_device(deviceinstance);

	return;
}

void pic_send_eoi(uint8_t irq){
	asm_out_b(PIC_PRIMARY_COMMAND, PIC_EOI);
	
	if ((irq + PIC_PRIMARY_VECTOR_OFFSET) >= PIC_SECONDARY_VECTOR_OFFSET){
		asm_out_b(PIC_SECONDARY_COMMAND, PIC_EOI);
	}
}