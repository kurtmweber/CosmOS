//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/usb/usb.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>

/*
* perform device instance specific init here
*/
void deviceInitUSB(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id,pci_dev->bars[0], dev->name);
}

void USBSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &deviceInitUSB;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = USB;
    devicemgr_set_device_description(deviceinstance, "USB");
    devicemgr_register_device(deviceinstance);
}

/**
* find all USB devices and register them
*/
void usb_devicemgr_register_devices() {
    pci_devicemgr_search_devicetype(PCI_CLASS_SERIAL,PCI_SERIAL_SUBCLASS_USB, &USBSearchCB);
}
