//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/bridge/intelpciisa/intelpciisa.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <dev/pci/pci.h>
#include <debug/assert.h>

struct intelisapca_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
* perform device instance specific init here
*/
void pciisa_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct intelisapca_deviceddata* deviceData = (struct intelisapca_deviceddata*) dev->deviceData;
    deviceData->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);
 }

void pciisa_search_cb(struct pci_device* dev){
    ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &pciisa_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "Intel PIIX3 PCI-to-ISA Bridge (Triton II)");
	/*
	* device data
	*/
	struct intelisapca_deviceddata* deviceData = (struct intelisapca_deviceddata*) kmalloc(sizeof(struct intelisapca_deviceddata));
	deviceinstance->deviceData = deviceData;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}

/**
* find all bridge devices and register them
*/
void pciisa_bridge_register() {
    pci_devicemgr_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_ISA, &pciisa_search_cb);
}
