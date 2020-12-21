/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/adlib/adlib.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <collection/list/list.h>
#include <interrupts/interrupt_router.h>


void adlib_handle_irq(stackFrame *frame) {
}

/*
* perform device instance specific init here
*/

void deviceInitADLIB(struct device* dev){
 //   kprintf("Init %s at IRQ %llu\n",dev->description, SB16_IRQ);
 //   interrupt_router_register_interrupt_handler(SB16_IRQ, &adlib_handle_irq);
}

void adlib_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "Yamaha YM3812 (OPL2)");
	deviceinstance->devicetype = DSP;
	deviceinstance->init =  &deviceInitADLIB;
	devicemgr_register_device(deviceinstance);
}

