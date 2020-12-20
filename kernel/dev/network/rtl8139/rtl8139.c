//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/network/rtl8139/rtl8139.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <dev/pci/pci.h>

void rtl8139_irq_handler(stackFrame *frame){

}

/*
* perform device instance specific init here
*/
void RTL8139Init(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    registerInterruptHandler(pci_dev->irq, &rtl8139_irq_handler);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id);
}

void RTL8139SearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = new_device();
    deviceinstance->init =  &RTL8139Init;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ETHERNET;
    device_set_description(deviceinstance, "Realtek RTL8139 10/100 NIC");
    register_device(deviceinstance);
}

/**
* find all RTL8139 devices and register them
*/
void rtl8139_register_devices() {
    pci_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x10EC,0x8139, &RTL8139SearchCB);
}
