//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/cpu/cpu.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>

/*
* perform device instance specific init here
*/

void deviceInitCPU(struct device* dev){
    kprintf("Init %s (%s)\n",dev->description, dev->name);
}

void cpu_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "CPU");
	deviceinstance->devicetype = CPU;
	deviceinstance->init =  &deviceInitCPU;
	devicemgr_register_device(deviceinstance);
}