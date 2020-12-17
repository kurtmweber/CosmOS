//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <network/ne2000/ne2000.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <pci/pci.h>

void ne2000_irq_handler(){
}
/*
* perform device instance specific init here
*/
void NE200Init(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    registerInterruptHandler(pci_dev->irq, &ne2000_irq_handler);
    kprintf("Init %s at IRQ %llu\n",dev->description, pci_dev->irq);
}

void NE2000SearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = newDevice();
    deviceinstance->init =  &NE200Init;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ETHERNET;
    deviceSetDescription(deviceinstance, "NE2000");
    registerDevice(deviceinstance);
}
/**
* find all NE2000 devices and register them
*/
void ne2000_register_devices() {
    pci_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x10EC,0x8029, &NE2000SearchCB);
}
