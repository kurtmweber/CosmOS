/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/dev_platform/i386/adlib/adlib.h>
#include <asm/asm.h>
#include <system/devicemgr/devicemgr.h>
#include <system/console/console.h>
#include <collection/list/list.h>
#include <system/system_platform/i386/interrupts/interrupt_router.h>
#include <system/devicemgr/deviceapi/deviceapi_dsp.h>
#include <debug/assert.h>

void adlib_handle_irq(stackFrame *frame) {
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
}

/*
* perform device instance specific init here
*/
void adlib_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
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
	deviceinstance->init =  &adlib_device_init;
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

