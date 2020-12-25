/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/sb16/sb16.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <collection/list/list.h>
#include <interrupts/interrupt_router.h>
#include <devicemgr/deviceapi/deviceapi_dsp.h>

// https://wiki.osdev.org/Sound_Blaster_16

#define SB16_IRQ        5
#define SB16_BASE       0x220
#define SB16_DMA1       1
#define SB16_DMA2       5

void sb16_handle_irq(stackFrame *frame) {
}

/*
* perform device instance specific init here
*/
void deviceInitSB16(struct device* dev){
    kprintf("Init %s at IRQ %llu (%s)\n",dev->description, SB16_IRQ, dev->name);
    interrupt_router_register_interrupt_handler(SB16_IRQ, &sb16_handle_irq);
}

void sb16_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "Soundblaster 16");
	deviceinstance->devicetype = DSP;
	deviceinstance->init =  &deviceInitSB16;
	/*
	* device api
	*/
	struct deviceapi_dsp* api = (struct deviceapi_dsp*) kmalloc (sizeof(struct deviceapi_dsp));
	deviceinstance->api = api;
	/*
	* register
	*/
	devicemgr_register_device(deviceinstance);
}

