//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <usb/usb.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>

/*
* perform device instance specific init here
*/
void deviceInitUSB(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu\n",dev->description, pci_dev->irq);
}

void USBSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = newDevice();
    deviceinstance->init =  &deviceInitUSB;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = USB;
    deviceSetDescription(deviceinstance, "USB");
    registerDevice(deviceinstance);
}

/**
* find all USB devices and register them
*/
void usb_register_devices() {
    pci_search_devicetype(PCI_CLASS_SERIAL,PCI_SERIAL_SUBCLASS_USB, &USBSearchCB);
}
