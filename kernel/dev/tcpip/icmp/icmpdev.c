//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ip.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct icmp_devicedata {
    struct device* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t icmp_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct icmp_devicedata* deviceData = (struct icmp_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->ethernet_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t icmp_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);

    return 1;
}

void icmp_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //  struct icmp_devicedata* deviceData = (struct icmp_devicedata*)dev->deviceData;
}
void icmp_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //   struct icmp_devicedata* deviceData = (struct icmp_devicedata*)dev->deviceData;
}

struct device* icmp_attach(struct device* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->devicetype == ETHERNET);

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &icmp_init;
    deviceinstance->uninit = &icmp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ICMP;
    devicemgr_set_device_description(deviceinstance, "Internet Control Message Protocol");
    /*
     * the device api
     */
    struct deviceapi_ip* api = (struct deviceapi_ip*)kmalloc(sizeof(struct deviceapi_ip));
    memzero((uint8_t*)api, sizeof(struct deviceapi_ip));
    api->read = &icmp_read;
    api->write = &icmp_write;

    deviceinstance->api = api;
    /*
     * device data
     */
    struct icmp_devicedata* deviceData = (struct icmp_devicedata*)kmalloc(sizeof(struct icmp_devicedata));
    deviceData->ethernet_device = ethernet_device;
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

void icmp_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}