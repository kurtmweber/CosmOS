//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/bridge/inteli440fx/inteli440fx.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <pci/pci.h>

/*
* perform device instance specific init here
*/
void deviceInitI440fx(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id);
}

void I440fxSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = newDevice();
    deviceinstance->init =  &deviceInitI440fx;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = BRIDGE;
    deviceSetDescription(deviceinstance, "Intel i440FX Chipset");
    registerDevice(deviceinstance);
}

/**
* find all bridge devices and register them
*/
void bridge_register_i440fx() {
    pci_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_HOST, &I440fxSearchCB);
}
