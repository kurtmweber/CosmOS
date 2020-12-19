//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/mouse/mouse.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>

/*
* perform device instance specific init here
*/
void deviceInitMouse(struct device* dev){
}

void MouseSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = newDevice();
    deviceinstance->init =  &deviceInitMouse;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = MOUSE;
    deviceSetDescription(deviceinstance, "Mouse");
    registerDevice(deviceinstance);
}

/**
* find all PS/2 mouse devices and register them
*/
void mouse_register_devices() {
}
