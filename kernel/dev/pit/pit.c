/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/pit/pit.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>


/*
* perform device instance specific init here
*/
void deviceInitPIT(struct device* dev){
    kprintf("Init %s\n" ,dev->description);

}

void pit_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = newDevice();
	deviceSetDescription(deviceinstance, "PIT");
	deviceinstance->devicetype = PIT;
	deviceinstance->init =  &deviceInitPIT;
	registerDevice(deviceinstance);

	return;
}
