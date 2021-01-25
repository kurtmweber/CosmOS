//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/network/e1000/e1000.h>
#include <dev/i386/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

void e1000_irq_handler(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
uint8_t e1000_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n", dev->description, dev->pci->irq, dev->pci->vendor_id,
            dev->pci->device_id, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &e1000_irq_handler);
        return 1;

}

void e1000_ethernet_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    panic("Ethernet read not implemented yet");
}
void e1000_ethernet_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    panic("Ethernet write not implemented yet");
}

void e1000_search_cb(struct pci_device* dev) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &e1000_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = NIC;
    devicemgr_set_device_description(deviceinstance, "E1000 NIC");
    /*
     * the device api
     */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*)kmalloc(sizeof(struct deviceapi_ethernet));
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
    pci_devicemgr_search_device(PCI_CLASS_NETWORK, PCI_NETWORK_SUBCLASS_ETHERNET, 0x8086, 0x100E, &e1000_search_cb);
}
