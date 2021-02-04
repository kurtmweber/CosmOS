/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/pci/pci.h>
#include <dev/x86-64/sdhci/sdhci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <sys/string/mem.h>

#define SDHCI_COMMAND 0x00
#define SDHCI_RESPONSE 0x10
#define SDHCI_RESPONSE 0x20
#define SDHCI_HOST_CONTROL 0x24
#define SDHCI_INTERRUPT 0x30

// https://www.sdcard.org/downloads/pls/

struct sdhci_devicedata {
    uint64_t base;
} __attribute__((packed));

void sdhci_irq_handler(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("?");
}

/*
 * perform device instance specific init here
 */
uint8_t sdhci_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sdhci_devicedata* deviceData = (struct sdhci_devicedata*)dev->deviceData;
    deviceData->base = CONV_PHYS_ADDR(pci_calcbar(dev->pci));
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &sdhci_irq_handler);
    return 1;
}

/*
 * perform device instance specific uninit here
 */
uint8_t sdhci_device_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    return 1;
}

void sdhci_pci_search_cb(struct pci_device* dev) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "SDHCI Controller");
    deviceinstance->devicetype = SDHCI;
    deviceinstance->pci = dev;
    deviceinstance->init = &sdhci_device_init;
    deviceinstance->uninit = sdhci_device_uninit;
    /*
     * device api
     */
    //   struct deviceapi_block* api = (struct deviceapi_block*)kmalloc(sizeof(struct deviceapi_block));
    //   memzero((uint8_t*)api, sizeof(struct deviceapi_block));
    //   deviceinstance->api = api;
    /*
     * the deviceData
     */
    struct sdhci_devicedata* deviceData = (struct sdhci_devicedata*)kmalloc(sizeof(struct sdhci_devicedata));
    deviceData->base = 0;
    deviceinstance->deviceData = deviceData;
    /**
     * register
     */
    devicemgr_register_device(deviceinstance);
}

/**
 * find all NE2000 devices and register them
 */
void sdhci_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_BASE_PERI, 0x05, 0x1B36, 0x07, &sdhci_pci_search_cb);
}
