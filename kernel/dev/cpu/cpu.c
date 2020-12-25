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
#include <devicemgr/deviceapi/deviceapi_cpu.h>

/*
* perform device instance specific init here
*/

void deviceInitCPU(struct device* dev){
    kprintf("Init %s (%s)\n",dev->description, dev->name);
}

uint64_t cpu_get_features() {
    return 12;
}


void cpu_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "CPU");
	deviceinstance->devicetype = CPU;
	deviceinstance->init =  &deviceInitCPU;
	/*
	* device api
	*/
	struct deviceapi_cpu* api = (struct deviceapi_cpu*) kmalloc (sizeof(struct deviceapi_cpu));
	api->features = &cpu_get_features;
	deviceinstance->api = api;
    /*
    * register
    */ 
	devicemgr_register_device(deviceinstance);
}