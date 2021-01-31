//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/fs_util.h>
#include <dev/partition/partition.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct partition_devicedata {
    struct device* block_device;
    uint32_t lba;
    uint32_t sector_count;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t partition_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct partition_devicedata* deviceData = (struct partition_devicedata*)dev->deviceData;
    kprintf("Init %s on %s at lba %llu with %llu sectors (%s)\n", dev->description, deviceData->block_device->name,
            deviceData->lba, deviceData->sector_count, dev->name);

    // attach fs
    fsutil_attach_fs(dev);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t partition_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct partition_devicedata* deviceData = (struct partition_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    // detach fs
    fsutil_detach_fs(dev);

    kfree(dev->api);
    kfree(dev->deviceData);
    return 1;
}

uint16_t partition_sector_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct partition_devicedata* deviceData = (struct partition_devicedata*)dev->deviceData;
    return block_get_sector_size(deviceData->block_device);
}

uint32_t partition_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct partition_devicedata* deviceData = (struct partition_devicedata*)dev->deviceData;
    return block_get_total_size(deviceData->block_device);
}

void partition_read_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    struct partition_devicedata* deviceData = (struct partition_devicedata*)dev->deviceData;
    return block_read_sectors(deviceData->block_device, deviceData->lba + sector, data, count);
}

void partition_write_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    struct partition_devicedata* deviceData = (struct partition_devicedata*)dev->deviceData;
    return block_write_sectors(deviceData->block_device, deviceData->lba + sector, data, count);
}

struct device* partition_attach(struct device* block_device, uint64_t lba, uint32_t sector_count) {
    ASSERT_NOT_NULL(block_device);
    ASSERT(block_device->devicetype == PARTITION_TABLE);

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &partition_init;
    deviceinstance->uninit = &partition_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARTITION;
    devicemgr_set_device_description(deviceinstance, "Partition");
    /*
     * the device api
     */
    struct deviceapi_block* api = (struct deviceapi_block*)kmalloc(sizeof(struct deviceapi_block));
    api->sector_size = &partition_sector_size;
    api->total_size = &partition_total_size;
    api->read = &partition_read_sector;
    api->write = &partition_write_sector;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct partition_devicedata* deviceData =
        (struct partition_devicedata*)kmalloc(sizeof(struct partition_devicedata));
    deviceData->block_device = block_device;
    deviceData->lba = lba;
    deviceData->sector_count = sector_count;
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

void partition_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}