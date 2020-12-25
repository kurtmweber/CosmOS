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
#include <devicemgr/deviceapi/deviceapi_ethernet.h>
#include <panic/panic.h>

void e1000_irq_handler(stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
}

/*
* perform device instance specific init here
*/
void E1000Init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &e1000_irq_handler);
}

void e1000_ethernet_read(struct device* dev, uint8_t* data, uint8_t* size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	panic("Ethernet read not implemented yet");
}
void e1000_ethernet_write(struct device* dev, uint8_t* data, uint8_t* size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	panic("Ethernet write not implemented yet");
}

void E1000SearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &E1000Init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "E1000 NIC");
    /*
    * the device api
    */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*) kmalloc(sizeof(struct deviceapi_ethernet));
    api->write = &e1000_ethernet_read;
    api->read = &e1000_ethernet_write;
    deviceinstance->api = api;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}

/**
*/
void e1000_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x8086,0x100E, &E1000SearchCB);
}
