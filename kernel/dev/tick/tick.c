//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_pit.h>
#include <sys/deviceapi/deviceapi_tick.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct tick_devicedata {
    struct device* pit_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t tick_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct tick_devicedata* deviceData = (struct tick_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->pit_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tick_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);

    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}

uint64_t tick_read(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct tick_devicedata* deviceData = (struct tick_devicedata*)dev->deviceData;

    struct deviceapi_pit* api = (struct deviceapi_pit*)deviceData->pit_device->api;
    return (*api->tickcount)(deviceData->pit_device);
}

struct device* tick_attach(struct device* pit_device) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &tick_init;
    deviceinstance->uninit = &tick_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = TICK;
    devicemgr_set_device_description(deviceinstance, "tick");
    /*
     * the device api
     */
    struct deviceapi_tick* api = (struct deviceapi_tick*)kmalloc(sizeof(struct deviceapi_tick));
    api->read = &tick_read;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct tick_devicedata* deviceData = (struct tick_devicedata*)kmalloc(sizeof(struct tick_devicedata));
    deviceData->pit_device = pit_device;
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
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void tick_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}