//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_null.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

/*
 * perform device instance specific init here
 */
uint8_t null_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t null_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}

uint8_t null_read(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    return 0;
}

struct device* null_attach() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &null_init;
    deviceinstance->uninit = &null_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = NULL0;
    devicemgr_set_device_description(deviceinstance, "null");
    /*
     * the device api
     */
    struct deviceapi_null* api = (struct deviceapi_null*)kmalloc(sizeof(struct deviceapi_null));
    memzero((uint8_t*)api, sizeof(struct deviceapi_null));
    api->read = &null_read;
    deviceinstance->api = api;
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

void null_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}