//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>
#include <debug/assert.h>
#include <dev/usb/usb_controller.h>
#include <dev/usb/ehci.h>
#include <dev/usb/ohci.h>

struct usbcontroller_devicedata {
    uint64_t base;
} __attribute__((packed));

/*
* perform device instance specific init here
*/
void usb_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    struct usbcontroller_devicedata* deviceData = (struct usbcontroller_devicedata*) dev->deviceData;
    deviceData->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);
}

void usb_search_cb(struct pci_device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &usb_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = USB;
    devicemgr_set_device_description(deviceinstance, "Intel 82801 USB Controller");
    /*
    * the deviceData
    */
    struct usbcontroller_devicedata* deviceData = (struct usbcontroller_devicedata*) kmalloc(sizeof(struct usbcontroller_devicedata));
    deviceData->base = 0;
    deviceinstance->deviceData = deviceData;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);  
}

/**
* find all USB devices and register them
*/
void usb_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_SERIAL,PCI_SERIAL_SUBCLASS_USB,0x8086, 0x24CD,  &usb_search_cb);
}
