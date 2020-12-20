//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/network/e1000/e1000.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <dev/pci/pci.h>

void e1000_irq_handler(stackFrame *frame){

}

/*
* perform device instance specific init here
*/
void E1000Init(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    register_interrupt_handler(pci_dev->irq, &e1000_irq_handler);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id);
}

void E1000SearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &E1000Init;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "E1000 NIC");
    devicemgr_register_device(deviceinstance);
}

/**
*/
void e1000_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x8086,0x100E, &E1000SearchCB);
}
