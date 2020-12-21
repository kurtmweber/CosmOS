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
    interrupt_router_register_interrupt_handler(pci_dev->irq, &rtl8139_irq_handler);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id, dev->name);
}

void RTL8139SearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &RTL8139Init;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "Realtek RTL8139 10/100 NIC");
    devicemgr_register_device(deviceinstance);
}

/**
* find all RTL8139 devices and register them
*/
void rtl8139_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x10EC,0x8139, &RTL8139SearchCB);
}
