//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/network/rtl8139/rtl8139.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/asm/asm.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>
#include <dev/i386/pci/pci.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/debug/assert.h>

void rtl8139_irq_handler(stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
}

/*
* perform device instance specific init here
*/
void rtl8139_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &rtl8139_irq_handler);
}

void rtl8139_ethernet_read(struct device* dev, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");

	panic("Ethernet read not implemented yet");
}
void rtl8139_ethernet_write(struct device* dev, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");

	panic("Ethernet write not implemented yet");
}

void rtl8139_search_cb(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &rtl8139_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = NIC;
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
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x10EC,0x8139, &rtl8139_search_cb);
}
