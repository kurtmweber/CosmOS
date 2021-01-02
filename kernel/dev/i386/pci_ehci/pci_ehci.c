/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/i386/pci_ehci/pci_ehci.h>
#include <asm/asm.h>
#include <system/devicemgr/devicemgr.h>
#include <system/console/console.h>
#include <collection/list/list.h>
#include <system/platform/i386/interrupts/interrupt_router.h>
#include <debug/assert.h>
#include <dev/i386/pci/pci.h>
#include <dev/i386/ehci/ehci.h>

// https://wiki.osdev.org/Enhanced_Host_Controller_Interface

void pci_ehci_handle_irq(stackFrame *frame) {
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
}

/*
* perform device instance specific init here
*/
void pci_ehci_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s at IRQ %llu\n",dev->description, dev->pci->irq);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &pci_ehci_handle_irq);
}

void pci_ehci_search_cb(struct pci_device* dev){
    ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &pci_ehci_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "PCI EHCI Controller");
	/*
	* device data
	*/
//	struct intelisapca_deviceddata* deviceData = (struct intelisapca_deviceddata*) kmalloc(sizeof(struct intelisapca_deviceddata));
//	deviceinstance->deviceData = deviceData;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}

/**
* find all bridge devices and register them
*/
void pci_ehci_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_BASE_PERI,0x05, 0x1B36,0x07, &pci_ehci_search_cb);
}

