//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 202q Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/swap/swap.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_swap.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <types.h>

/*
 * swap instance specific data
 */
struct swap_devicedata {
    uint64_t block_size;
    uint64_t blocks;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
void swap_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct swap_devicedata* deviceData = (struct swap_devicedata*)dev->deviceData;
    kprintf("Init %s (%s)\n", dev->description, dev->name);
}

void swap_read(struct device* dev, uint8_t* data, uint32_t block) {
}

void swap_write(struct device* dev, uint8_t* data, uint32_t block) {
}

void swap_mount(struct device* dev) {
}

/**
 *  register
 */
void swap_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &swap_init;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = SWAP;
    devicemgr_set_device_description(deviceinstance, "Swap");
    /*
     * device data
     */
    struct swap_devicedata* deviceData = (struct swap_devicedata*)kmalloc(sizeof(struct swap_devicedata));
    deviceData->block_size = 0;
    deviceData->blocks = 0;
    deviceinstance->deviceData = deviceData;
    /*
     * the device api
     */
    struct deviceapi_swap* api = (struct deviceapi_swap*)kmalloc(sizeof(struct deviceapi_swap));
    api->write = &swap_write;
    api->read = &swap_read;
    api->mount = &swap_mount;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}
