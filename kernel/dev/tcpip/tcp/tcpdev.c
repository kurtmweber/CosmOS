//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_tcp.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct tcp_devicedata {
    struct device* ip_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t tcp_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct tcp_devicedata* deviceData = (struct tcp_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->ip_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tcp_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);

    return 1;
}

void tcp_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //  struct ip_devicedata* deviceData = (struct ip_devicedata*)dev->deviceData;
}
void tcp_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //   struct ip_devicedata* deviceData = (struct ip_devicedata*)dev->deviceData;
}

struct device* tcp_attach(struct device* ip_device) {
    ASSERT_NOT_NULL(ip_device);
    ASSERT(ip_device->devicetype == IP);

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &tcp_init;
    deviceinstance->uninit = &tcp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = TCP;
    devicemgr_set_device_description(deviceinstance, "Transmission Control Protocol");
    /*
     * the device api
     */
    struct deviceapi_tcp* api = (struct deviceapi_tcp*)kmalloc(sizeof(struct deviceapi_tcp));
    memzero((uint8_t*)api, sizeof(struct deviceapi_tcp));
    api->read = &tcp_read;
    api->write = &tcp_write;

    deviceinstance->api = api;
    /*
     * device data
     */
    struct tcp_devicedata* deviceData = (struct tcp_devicedata*)kmalloc(sizeof(struct tcp_devicedata));
    deviceData->ip_device = ip_device;
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
        kfree(deviceData);
        kfree(deviceinstance);
        return 0;
    }
}

void tcp_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}