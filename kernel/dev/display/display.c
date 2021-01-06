//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/display/display.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>
#include <debug/assert.h>

/*
* perform device instance specific init here
*/
void display_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, dev->name);
}

void display_search_cb(struct pci_device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &display_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = VGA;
    devicemgr_set_device_description(deviceinstance, "QEMU/Bochs VBE Framebuffer");
    devicemgr_register_device(deviceinstance);
}

/**
* find all Display devices and register them
*/
void display_devicemgr_register_devices() {
    pci_devicemgr_search_devicetype(PCI_CLASS_DISPLAY,PCI_DISPLAY_SUBCLASS_VGA, &display_search_cb);
}
