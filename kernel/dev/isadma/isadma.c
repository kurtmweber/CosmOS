//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/isadma/isadma.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>

// https://wiki.osdev.org/DMA

/*
* perform device instance specific init here
*/
void deviceInitISADMA(struct device* dev){
    kprintf("Init %s\n",dev->description);
}

void isadma_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "8237 ISA DMA");
	deviceinstance->devicetype = DMA;
	deviceinstance->init =  &deviceInitISADMA;
	devicemgr_register_device(deviceinstance);
}