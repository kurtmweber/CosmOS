//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_arp.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/panic/panic.h>
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
    kfree(dev->deviceData);

    return 1;
}

/*
* perform an ARP request
*/
void arp_request(struct device* dev, struct arp* request, struct arp* response) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    // get our device data for this device
    //   struct arp_devicedata* deviceData = (struct arp_devicedata*)dev->deviceData;
    // get the api for the underlying ethernet device
    //   struct deviceapi_ethernet* ether_api = (struct deviceapi_ethernet*)deviceData->ethernet_device->api;
    panic("Um, what HW addresses do I put in here?");
    // send
    //  (*ether_api->write)(deviceData->ethernet_device, (uint8_t*)request, sizeof(struct arp));
    // receive. I presume this blocks?
    // (*ether_api->read)(deviceData->ethernet_device, (uint8_t*)response, sizeof(struct arp));
}

struct device* arp_attach(struct device* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->devicetype == ETHERNET);

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

void arp_packet_init(struct arp* a, uint16_t opcode) {
    ASSERT_NOT_NULL(a);
    ASSERT(sizeof(struct arp) == 96);
    memset((uint8_t*)a, 0, sizeof(struct arp));
    a->htype = ARP_ETHERNET;
    a->ptype = ARP_IP;
    a->hlen = ARP_HLEN;
    a->plen = ARP_PLEN;
    a->opcode = opcode;
}
