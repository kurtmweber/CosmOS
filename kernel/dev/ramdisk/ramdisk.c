//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/ramdisk/ramdisk.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

/*
 * ramdisk instance specific data
 */
struct ramdisk_devicedata {
    uint8_t* data;
    uint64_t size;
    uint16_t sector_size;
    uint16_t sector_count;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
void ramdisk_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*)dev->deviceData;
    kprintf("Init %s of size %llu MB at %#hX (%s)\n", dev->description, (deviceData->size) / 1024, deviceData->data,
            dev->name);
    memset(deviceData->data, 0, deviceData->size);
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void ramdisk_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*)dev->deviceData;
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(deviceData->data);
    kfree(deviceData);
    kfree(dev->api);
}

uint64_t ramdisk_calc_address(struct ramdisk_devicedata* deviceData, uint32_t sector) {
    return (uint64_t)(deviceData->data) + (sector * deviceData->sector_size);
}

void ramdisk_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*)dev->deviceData;
    ASSERT(count <= deviceData->sector_size);
    ASSERT(sector < deviceData->sector_count);
    uint64_t block = ramdisk_calc_address(deviceData, sector);
    kprintf("block %#hX\n", block);
    memcpy((uint8_t*)data, (uint8_t*)block, count * sizeof(uint8_t));
}

void ramdisk_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*)dev->deviceData;
    ASSERT(count <= deviceData->sector_size);
    ASSERT(sector < deviceData->sector_count);
    uint64_t block = ramdisk_calc_address(deviceData, sector);
    kprintf("block %#hX\n", block);
    memcpy((uint8_t*)block, (uint8_t*)data, count * sizeof(uint8_t));
}

uint16_t ramdisk_sector_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*)dev->deviceData;

    return deviceData->sector_size;
}
uint32_t ramdisk_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*)dev->deviceData;

    return deviceData->size;
}

struct device* ramdisk_attach(uint16_t sector_size, uint16_t sector_count) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &ramdisk_init;
    deviceinstance->uninit = &ramdisk_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = RAMDISK;
    devicemgr_set_device_description(deviceinstance, "RAM disk");
    /*
     * device data
     */
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*)kmalloc(sizeof(struct ramdisk_devicedata));
    deviceData->size = (uint64_t)sector_size * sector_count;
    deviceData->sector_size = sector_size;
    deviceData->sector_count = sector_count;
    deviceData->data = kmalloc(deviceData->size);
    deviceinstance->deviceData = deviceData;
    /*
     * the device api
     */
    struct deviceapi_block* api = (struct deviceapi_block*)kmalloc(sizeof(struct deviceapi_block));
    api->write = &ramdisk_write;
    api->read = &ramdisk_read;
    api->sector_size = &ramdisk_sector_size;
    api->total_size = &ramdisk_total_size;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_attach_device(deviceinstance);

    /*
     * return device
     */
    return deviceinstance;
}

void ramdisk_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}
