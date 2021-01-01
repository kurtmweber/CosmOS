/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/ehcibus/ehcibus.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <collection/list/list.h>
#include <interrupts/interrupt_router.h>
#include <debug/assert.h>
#include <dev/pci/pci.h>

void ehcibus_handle_irq(stackFrame *frame) {
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
}

/*
* perform device instance specific init here
*/
void ehcibus_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s at IRQ %llu\n",dev->description, dev->pci->irq);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &ehcibus_handle_irq);
}

void ehcibus_search_cb(struct pci_device* dev){
    ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &ehcibus_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "EHCI bus");
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
void ehcibus_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_MASS_STORAGE,PCI_MASS_STORAGE_SUBCLASS_SCSI, 0x1AF4,0x1001, &ehcibus_search_cb);
}

