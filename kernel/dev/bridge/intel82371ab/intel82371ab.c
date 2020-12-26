//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/bridge/intel82371ab/intel82371ab.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <dev/pci/pci.h>
#include <panic/assert.h>


struct intel8237_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
* perform device instance specific init here
*/
void deviceInit82371Bridge(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct intel8237_deviceddata* deviceData = (struct intel8237_deviceddata*) dev->deviceData;
    deviceData->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);
}

void Bridge82371SearchCB(struct pci_device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &deviceInit82371Bridge;
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
void bridge_register_82371() {
    pci_devicemgr_search_devicetype(PCI_CLASS_BRIDGE,PCI_BRIDGE_SUBCLASS_OTHER, &Bridge82371SearchCB);
}
