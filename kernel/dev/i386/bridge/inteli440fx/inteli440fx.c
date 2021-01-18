//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/bridge/inteli440fx/inteli440fx.h>
#include <dev/i386/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

struct intel440fx_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
void i440fx_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct intel440fx_deviceddata* deviceData = (struct intel440fx_deviceddata*)dev->deviceData;
    deviceData->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq, dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);
}

void i440fx_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &i440fx_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "Intel i440FX PCI Bridge");
    /*
     * device data
     */
    struct intel440fx_deviceddata* deviceData = (struct intel440fx_deviceddata*)kmalloc(sizeof(struct intel440fx_deviceddata));
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

/**
 * find all bridge devices and register them
 */
void i440fx_bridge_register() {
    pci_devicemgr_search_devicetype(PCI_CLASS_BRIDGE, PCI_BRIDGE_SUBCLASS_HOST, &i440fx_search_cb);
}
