/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/ehci/ehci.h>
#include <dev/x86-64/pci/pci.h>
#include <dev/x86-64/pci_ehci/pci_ehci.h>
#include <sys/asm/asm.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

// https://wiki.osdev.org/Enhanced_Host_Controller_Interface

void pci_ehci_handle_irq(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
uint8_t pci_ehci_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s at IRQ %llu\n", dev->description, dev->pci->irq);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &pci_ehci_handle_irq);
    return 1;
}

void pci_ehci_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &pci_ehci_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "PCI EHCI Controller");
    /*
     * device data
     */
    //	struct intelisapca_deviceddata* deviceData = (struct intelisapca_deviceddata*) kmalloc(sizeof(struct intelisapca_deviceddata));
    //	deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

/**
 * find all bridge devices and register them
 */
void pci_ehci_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_BASE_PERI, 0x05, 0x1B36, 0x07, &pci_ehci_search_cb);
}
