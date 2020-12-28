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
#include <devicemgr/deviceapi/deviceapi_ethernet.h>
#include <debug/assert.h>

void rtl8139_irq_handler(stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
}

/*
* perform device instance specific init here
*/
void RTL8139Init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &rtl8139_irq_handler);
}

void rtl8139_ethernet_read(struct device* dev, uint8_t* data, uint8_t* size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	panic("Ethernet read not implemented yet");
}
void rtl8139_ethernet_write(struct device* dev, uint8_t* data, uint8_t* size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	panic("Ethernet write not implemented yet");
}

void RTL8139SearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &RTL8139Init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "Realtek RTL8139 10/100 NIC");
    /*
    * the device api
    */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*) kmalloc(sizeof(struct deviceapi_ethernet));
    api->write = &rtl8139_ethernet_read;
    api->read = &rtl8139_ethernet_write;
    deviceinstance->api = api;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}

/**
* find all RTL8139 devices and register them
*/
void rtl8139_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x10EC,0x8139, &RTL8139SearchCB);
}
