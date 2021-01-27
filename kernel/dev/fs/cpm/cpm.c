//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/cpm/cpm.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

// https://obsolescence.wixsite.com/obsolescence/cpm-internals

#define CPM_MAX_FILES 64
#define CPM_FILE_ENTRY_LEN 32
#define CPM_SECTOR_SIZE 128
#define CPM_EMPTY 0xe5

struct cpm_file_entry {
    uint8_t used;
    uint8_t filename[8];
    uint8_t extension[3];
    uint8_t ex;
    uint8_t unused;
    uint8_t record_count;
    uint8_t disk_map[16];
    uint8_t cr;
    //   uint8_t dma[3];
} __attribute__((packed));

struct cpm_dir {
    struct cpm_file_entry files[CPM_MAX_FILES];
} __attribute__((packed));

struct cpm_devicedata {
    struct device* block_device;
    uint16_t block_device_sector_size;
    uint8_t sectors_needed_for_dir;
    uint32_t byte_size_dir;
} __attribute__((packed));

void cpm_format(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct cpm_devicedata* deviceData = (struct cpm_devicedata*)dev->deviceData;

    /*
    * make the dir entry
    */
    struct cpm_dir dir;
    memset((uint8_t*)&dir, 0, sizeof(struct cpm_dir));
    for (uint8_t i = 0; i < CPM_MAX_FILES; i++) {
        dir.files[i].used = CPM_EMPTY;
    }

    /*
    * figure out how many sectors we need
    */
    uint8_t* buffer = kmalloc(deviceData->byte_size_dir);
    memset(buffer, 0, deviceData->byte_size_dir);
    memcpy(buffer, (uint8_t*)&dir, sizeof(struct cpm_dir));
    for (uint8_t i = 0; i < deviceData->sectors_needed_for_dir; i++) {
        block_write(deviceData->block_device, i, &(buffer[i]));
    }
    kfree(buffer);
}

void cpm_read_dir(struct device* dev, struct cpm_dir* dir) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct cpm_devicedata* deviceData = (struct cpm_devicedata*)dev->deviceData;

    uint8_t* buffer = kmalloc(deviceData->byte_size_dir);
    memset(buffer, 0, deviceData->byte_size_dir);
    for (uint8_t i = 0; i < deviceData->sectors_needed_for_dir; i++) {
        block_read(deviceData->block_device, i, &(buffer[i]));
    }

    // copy to dest
    memcpy((uint8_t*)dir, buffer, sizeof(struct cpm_dir));

    kfree(buffer);
}

/*
 * perform device instance specific init here
 */
uint8_t cpm_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct cpm_devicedata* deviceData = (struct cpm_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t cpm_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct cpm_devicedata* deviceData = (struct cpm_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
    return 1;
}

struct device* cpm_attach(struct device* block_device) {
    ASSERT(sizeof(struct cpm_file_entry) == CPM_FILE_ENTRY_LEN);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &cpm_init;
    deviceinstance->uninit = &cpm_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    devicemgr_set_device_description(deviceinstance, "CP/M File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    api->format = &cpm_format;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct cpm_devicedata* deviceData = (struct cpm_devicedata*)kmalloc(sizeof(struct cpm_devicedata));
    deviceData->block_device = block_device;
    deviceData->block_device_sector_size = block_get_sector_size(deviceData->block_device);
    // blocks needed for dir
    uint8_t sectors_needed = sizeof(struct cpm_dir) / deviceData->block_device_sector_size;
    if (0 != (sizeof(struct cpm_dir) % deviceData->block_device_sector_size)) {
        sectors_needed = sectors_needed + 1;
    }
    deviceData->sectors_needed_for_dir = sectors_needed;
    deviceData->byte_size_dir = deviceData->sectors_needed_for_dir * deviceData->block_device_sector_size;
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

void cpm_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}
