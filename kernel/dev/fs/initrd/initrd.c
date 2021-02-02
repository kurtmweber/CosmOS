//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>

struct initrd_devicedata {
    struct device* partition_device;
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

struct device* initrd_attach(struct device* partition_device) {
    ASSERT_NOT_NULL(partition_device);
    ASSERT(partition_device->devicetype == PARTITION);

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
    api->format = 0;
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