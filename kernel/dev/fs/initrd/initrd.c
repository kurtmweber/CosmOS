//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/fs_util.h>
#include <dev/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>

#define INITRD_MAX_FILES 64
#define INITRD_MAX_FILENAME 64
#define INITRD_MAX_SECTORS 32
#define INITRD_MAGIC_SIZE 4
const uint8_t INITRD_MAGIC[] = {'t', 'g', 'e', '7'};

struct initrd_devicedata {
    struct device* partition_device;
} __attribute__((packed));

struct initrd_file {
    uint8_t name[INITRD_MAX_FILENAME];
    uint64_t lbas[INITRD_MAX_SECTORS];
} __attribute__((packed));

struct initrd_filetable {
    uint8_t magic[INITRD_MAGIC_SIZE];
    uint64_t header_sectors;
    uint32_t number_files;
    struct initrd_file files[INITRD_MAX_FILES];
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t initrd_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct initrd_devicedata* deviceData = (struct initrd_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->partition_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t initrd_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);

    struct initrd_devicedata* deviceData = (struct initrd_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->partition_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
    return 1;
}

void initrd_read_filetable(struct device* dev, struct initrd_filetable* filetable) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    ASSERT_NOT_NULL(filetable);
    struct initrd_devicedata* deviceData = (struct initrd_devicedata*)dev->deviceData;
    /*
    * read 1st sector to get header_sectors
    */
    uint32_t sector_size = block_get_sector_size(deviceData->partition_device);
    uint8_t datablock[sector_size];
    memzero(datablock, sector_size);
    block_read_sectors(deviceData->partition_device, 0, datablock, 1);
    debug_show_memblock(datablock, sector_size);
    uint32_t total_header_sectors = ((struct initrd_filetable*)datablock)->header_sectors;
    kprintf("ts %llu\n", total_header_sectors);
}

void initrd_write_filetable(struct device* dev, struct initrd_filetable* filetable) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    ASSERT_NOT_NULL(filetable);
    struct initrd_devicedata* deviceData = (struct initrd_devicedata*)dev->deviceData;
    /*
    * figure out how many sectors
    */
    uint32_t sector_size = block_get_sector_size(deviceData->partition_device);
    filetable->header_sectors = sizeof(struct initrd_filetable) / sector_size;
    if ((sizeof(struct initrd_filetable) % sector_size) != 0) {
        filetable->header_sectors += 1;
    }
    /*
    * write it
    */
    uint8_t datablock[sector_size * filetable->header_sectors];
    memzero(datablock, sector_size * filetable->header_sectors);
    memcpy(datablock, (uint8_t*)&filetable, sizeof(struct initrd_filetable));
    block_write_sectors(deviceData->partition_device, 0, datablock, filetable->header_sectors);

    struct initrd_filetable filetable2;
    memzero((uint8_t*)&filetable2, sizeof(struct initrd_filetable));
    initrd_read_filetable(dev, &filetable2);
}

void initrd_fs_format(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    /*
    * create the header struct
    */
    struct initrd_filetable filetable;
    memzero((uint8_t*)&filetable, sizeof(struct initrd_filetable));
    for (uint8_t i = 0; i < INITRD_MAGIC_SIZE; i++) {
        filetable.magic[i] = INITRD_MAGIC[i];
    }
    /*
    * write it
    */
    initrd_write_filetable(dev, &filetable);
}

struct device* initrd_attach(struct device* partition_device) {
    ASSERT_NOT_NULL(partition_device);
    // basically the device needs to implement deviceapi_block
    ASSERT((partition_device->devicetype == PARTITION) || (partition_device->devicetype == VBLOCK) ||
           (partition_device->devicetype == DISK) || (partition_device->devicetype == RAMDISK));

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &initrd_init;
    deviceinstance->uninit = &initrd_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    devicemgr_set_device_description(deviceinstance, "initrd File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    memzero((uint8_t*)api, sizeof(struct deviceapi_filesystem));
    api->format = &initrd_fs_format;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct initrd_devicedata* deviceData = (struct initrd_devicedata*)kmalloc(sizeof(struct initrd_devicedata));
    deviceData->partition_device = partition_device;
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

void initrd_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}