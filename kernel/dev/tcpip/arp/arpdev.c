//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_arp.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct arp_devicedata {
    struct device* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t arp_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct arp_devicedata* deviceData = (struct arp_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->ethernet_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t arp_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}

void arp_request(struct device* dev, struct arp* request, struct arp* response) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //  struct arp_devicedata* deviceData = (struct arp_devicedata*)dev->deviceData;
}

struct device* arp_attach(struct device* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT((ethernet_device->devicetype == NIC) || (ethernet_device->devicetype == VNIC));

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &arp_init;
    deviceinstance->uninit = &arp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ARP;
    devicemgr_set_device_description(deviceinstance, "Address Resolution Protocol");
    /*
     * the device api
     */
    struct deviceapi_arp* api = (struct deviceapi_arp*)kmalloc(sizeof(struct deviceapi_arp));
    memzero((uint8_t*)api, sizeof(struct deviceapi_arp));
    api->request = &arp_request;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct arp_devicedata* deviceData = (struct arp_devicedata*)kmalloc(sizeof(struct arp_devicedata));
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

void arp_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}

struct arp* arp_new(uint16_t opcode) {
    struct arp* ret = (struct arp*)kmalloc(sizeof(struct arp));
    memset((uint8_t*)ret, 0, sizeof(struct arp));
    ret->htype = ARP_ETHERNET;
    ret->ptype = ARP_IP;
    ret->hlen = ARP_HLEN;
    ret->plen = ARP_PLEN;
    ret->opcode = opcode;
    return ret;
}
