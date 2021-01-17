//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
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
 * perform device instance specific init here
 */
void swap_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void swap_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
}

void swap_read(struct device* dev, uint8_t* data, uint32_t block) {
}
void swap_write(struct device* dev, uint8_t* data, uint32_t block) {
}

struct device* swap_attach(struct device* block_device) {
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
    api->write = &swap_write;
    api->read = &swap_read;
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

void swap_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}
