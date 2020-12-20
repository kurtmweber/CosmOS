/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/pit/pit.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>

#define PIT_PORT_0        0x40
#define PIT_PORT_1        0x41
#define PIT_PORT_2        0x42
#define PIT_PORT_COMMAND  0x43


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
	deviceSetDescription(deviceinstance, " 8253/8254 PIT");
	deviceinstance->devicetype = PIT;
	deviceinstance->init =  &deviceInitPIT;
	registerDevice(deviceinstance);
}
