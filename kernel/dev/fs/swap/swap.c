//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/swap/swap.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_swap.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

struct swap_devicedata {
    struct device* block_device;
    uint16_t block_size;  // block size of the underlying physical device
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t swap_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct swap_devicedata* deviceData = (struct swap_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t swap_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct swap_devicedata* deviceData = (struct swap_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
    return 1;
}

void swap_read(struct device* dev, uint8_t* data, uint32_t block) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct swap_devicedata* deviceData = (struct swap_devicedata*)dev->deviceData;
    block_read(deviceData->block_device, block, data);
}

void swap_write(struct device* dev, uint8_t* data, uint32_t block) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct swap_devicedata* deviceData = (struct swap_devicedata*)dev->deviceData;
    block_write(deviceData->block_device, block, data);
}

uint16_t swap_block_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct swap_devicedata* deviceData = (struct swap_devicedata*)dev->deviceData;
    return block_get_sector_size(deviceData->block_device);
}

uint16_t swap_block_count(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct swap_devicedata* deviceData = (struct swap_devicedata*)dev->deviceData;
    return block_get_sector_count(deviceData->block_device);
}

struct device* swap_attach(struct device* block_device) {
    ASSERT_NOT_NULL(block_device);
    // basically the device needs to implement deviceapi_block
    ASSERT((block_device->devicetype == PARTITION) || (block_device->devicetype == VBLOCK) ||
           (block_device->devicetype == DISK) || (block_device->devicetype == RAMDISK));

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &swap_init;
    deviceinstance->uninit = &swap_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = SWAP;
    devicemgr_set_device_description(deviceinstance, "Swap");
    /*
     * the device api
     */
    struct deviceapi_swap* api = (struct deviceapi_swap*)kmalloc(sizeof(struct deviceapi_swap));
    memzero((uint8_t*)api, sizeof(struct deviceapi_swap));
    api->write = &swap_write;
    api->read = &swap_read;
    api->block_count = &swap_block_count;
    api->block_size = &swap_block_size;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct swap_devicedata* deviceData = (struct swap_devicedata*)kmalloc(sizeof(struct swap_devicedata));
    deviceData->block_device = block_device;
    deviceData->block_size = block_get_sector_size(block_device);
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(deviceData);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void swap_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}
