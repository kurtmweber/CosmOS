//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/bridge/intel82371ab/intel82371ab.h>
#include <system/platform/i386/interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <system/devicemgr/devicemgr.h>
#include <system/console/console.h>
#include <dev/i386/pci/pci.h>
#include <debug/assert.h>


struct intel8237_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
* perform device instance specific init here
*/
void i982371_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct intel8237_deviceddata* deviceData = (struct intel8237_deviceddata*) dev->deviceData;
    deviceData->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);
}

void i982371_search_cb(struct pci_device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &i982371_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "Intel PIIX4/4E/4M Power Management Controller");
	/*
	* device data
	*/
	struct intel8237_deviceddata* deviceData = (struct intel8237_deviceddata*) kmalloc(sizeof(struct intel8237_deviceddata));
	deviceinstance->deviceData = deviceData;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}

/**
* find all bridge devices and register them
*/
void i982371_bridge_register() {
    pci_devicemgr_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_OTHER, &i982371_search_cb);
}
