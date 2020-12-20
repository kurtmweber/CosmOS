/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/speaker/speaker.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>

/*
* perform device instance specific init here
*/
void deviceInitSpeaker(struct device* dev){
    kprintf("Init %s\n" ,dev->description);
}

void speaker_register_devices(){

    /*
	* register device
	*/
	struct device* deviceinstance = newDevice();
	deviceSetDescription(deviceinstance, "Speaker");
	deviceinstance->devicetype = SPEAKER;
	deviceinstance->init =  &deviceInitSpeaker;
	registerDevice(deviceinstance);

	return;
}
