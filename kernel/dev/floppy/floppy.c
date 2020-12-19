//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/floppy/floppy.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>

#define FLOPPY_IRQ_NUMBER 6

/*
* perform device instance specific init here
*/
void deviceInitFloppy(struct device* dev){
    kprintf("Init %s at IRQ %llu\n",dev->description, FLOPPY_IRQ_NUMBER);
}

/**
* find all floppy devices and register them
*/
void floppy_register_devices() {
    /*
	* register device
	*/
	struct device* deviceinstance = newDevice();
	deviceinstance->init =  &deviceInitFloppy;
	deviceinstance->devicetype = FLOPPY;
	deviceSetDescription(deviceinstance, "Floppy");
	registerDevice(deviceinstance);
}
