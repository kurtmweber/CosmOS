//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/deviceapi/deviceapi_nic.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct ethernet_devicedata {
    struct device* nic_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ethernet_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ethernet_devicedata* deviceData = (struct ethernet_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->nic_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ethernet_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}

void ethernet_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ethernet_devicedata* deviceData = (struct ethernet_devicedata*)dev->deviceData;
    panic("Not Implemented");
}

void ethernet_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ethernet_devicedata* deviceData = (struct ethernet_devicedata*)dev->deviceData;
    panic("Not Implemented");
}

struct device* ethernet_attach(struct device* nic_device) {
    ASSERT_NOT_NULL(nic_device);
    ASSERT((nic_device->devicetype == NIC) || (nic_device->devicetype == VNIC));

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &ethernet_init;
    deviceinstance->uninit = &ethernet_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "Ethernet");
    /*
     * the device api
     */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*)kmalloc(sizeof(struct deviceapi_ethernet));
    memzero((uint8_t*)api, sizeof(struct deviceapi_ethernet));
    api->read = &ethernet_read;
    api->write = &ethernet_write;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct ethernet_devicedata* deviceData = (struct ethernet_devicedata*)kmalloc(sizeof(struct ethernet_devicedata));
    deviceData->nic_device = nic_device;
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

void ethernet_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}
