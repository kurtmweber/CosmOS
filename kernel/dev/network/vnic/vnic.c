//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio*/

#include <dev/network/vnic/vnic.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <types.h>
#include <asm/io.h>
#include <sleep/sleep.h>
#include <dev/pci/pci.h>


void vnic_irq_handler(stackFrame *frame){
	kprintf("#");
}
/*
* perform device instance specific init here
*/
void VNICInit(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    interrupt_router_register_interrupt_handler(pci_dev->irq, &vnic_irq_handler);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id, dev->name);
}

void VNICSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &VNICInit;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "Virtio NIC");
    devicemgr_register_device(deviceinstance);
}

/**
* find all NE2000 devices and register them
*/
void vinc_devicemgr_register_devices() {
        pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x1AF4,0x1000, &VNICSearchCB);
}
