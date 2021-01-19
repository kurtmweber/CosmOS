//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/partition/partition.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_partition.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct parition_devicedata {
    struct device* block_device;
    uint32_t lba;
    uint32_t sector_count;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
void partition_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct parition_devicedata* deviceData = (struct parition_devicedata*)dev->deviceData;
    kprintf("Init %s on %s at lba %llu with %llu sectors (%s)\n", dev->description, deviceData->block_device->name, deviceData->lba, deviceData->sector_count, dev->name);
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void partition_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct parition_devicedata* deviceData = (struct parition_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
}

uint64_t partition_lba(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct parition_devicedata* deviceData = (struct parition_devicedata*)dev->deviceData;
    return deviceData->lba;
}

struct device* partition_device(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct parition_devicedata* deviceData = (struct parition_devicedata*)dev->deviceData;
    return deviceData->block_device;
}

void partition_read_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    struct parition_devicedata* deviceData = (struct parition_devicedata*)dev->deviceData;
    return block_read_sectors(deviceData->block_device, deviceData->lba + sector, data, count);
}

void partition_write_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    struct parition_devicedata* deviceData = (struct parition_devicedata*)dev->deviceData;
    return block_write_sectors(deviceData->block_device, deviceData->lba + sector, data, count);
}

struct device* partition_attach(struct device* block_device, uint64_t lba, uint32_t sector_count) {
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
    struct deviceapi_partition* api = (struct deviceapi_partition*)kmalloc(sizeof(struct deviceapi_partition));
    api->device = &partition_device;
    api->lba = &partition_lba;
    api->read = &partition_read_sector;
    api->write = &partition_write_sector;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct parition_devicedata* deviceData = (struct parition_devicedata*)kmalloc(sizeof(struct parition_devicedata));
    deviceData->block_device = block_device;
    deviceData->lba = lba;
    deviceData->sector_count = sector_count;
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_attach_device(deviceinstance);

    /*
     * return device
     */
    return deviceinstance;
}

void partition_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}