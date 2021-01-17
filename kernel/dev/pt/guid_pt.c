//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/GPT

#include <dev/fs/block_util.h>
#include <dev/pt/guid_pt.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_part_table.h>
#include <sys/kmalloc/kmalloc.h>

#define GUID_PT_HEADER_LBA 1

const uint8_t GUID_PT_EFI_PART[] = {0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54};

struct guid_pt_devicedata {
    struct device* block_device;
} __attribute__((packed));

void guid_pt_read_guid_pt_header(struct device* dev, struct guid_pt_header* header) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(header);

    block_read(dev, GUID_PT_HEADER_LBA, (uint8_t*)header, sizeof(struct guid_pt_header));
    ASSERT(header->magic[0] == GUID_PT_EFI_PART[0]);
}

/*
 * perform device instance specific init here
 */
void guid_pt_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void guid_pt_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);

    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
}

uint8_t guid_pt_part_table_total_partitions(struct device* dev) {
}

struct device* guid_pt_attach(struct device* block_device) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &guid_pt_init;
    deviceinstance->uninit = guid_pt_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARITION_TABLE;
    devicemgr_set_device_description(deviceinstance, "GUID parition table");
    /*
     * the device api
     */
    struct deviceapi_part_table* api = (struct deviceapi_part_table*)kmalloc(sizeof(struct deviceapi_part_table));
    api->parititions = &guid_pt_part_table_total_partitions;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)kmalloc(sizeof(struct guid_pt_devicedata));
    deviceData->block_device = block_device;
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_attach_device(deviceinstance);

    /*
     * return device
     */
    return deviceinstance;
}

void guid_pt_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}