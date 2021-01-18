//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Partition_Table
// https://wiki.osdev.org/MBR_(x86)

#include <dev/fs/block_util.h>
#include <dev/pt/mbr_pt.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_part_table.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

#define MBR_HEADER_LBA 0

struct mbr_pt_devicedata {
    struct device* block_device;
} __attribute__((packed));

void mbr_pt_read_mbr_pt_header(struct device* dev, struct mbr_pt_header* header) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(header);
    uint8_t buffer[512];
    block_read(dev, MBR_HEADER_LBA, buffer, 1);
    debug_show_memblock((uint8_t*)buffer, 512);
    memcpy((uint8_t*)header, buffer, sizeof(struct mbr_pt_header));
    kprintf("sig bytes %llu %llu\n", header->signature[0], header->signature[1]);
    ASSERT(header->signature[0] == 0x55);
    ASSERT(header->signature[1] == 0xAA);
}

/*
 * perform device instance specific init here
 */
void mbr_pt_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void mbr_pt_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);

    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
}

uint8_t mbr_pt_part_table_total_partitions(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(deviceData->block_device, &header);

    uint8_t ret = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (header.partitions[i].lba_start != 0) {
            ret = ret + 1;
        }
    }
    return ret;
}

struct device* mbr_pt_attach(struct device* block_device) {
    ASSERT(sizeof(struct mbr_pt_entry) == 16);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &mbr_pt_init;
    deviceinstance->uninit = mbr_pt_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARTITION_TABLE;
    devicemgr_set_device_description(deviceinstance, "MBR parition table");
    /*
     * the device api
     */
    struct deviceapi_part_table* api = (struct deviceapi_part_table*)kmalloc(sizeof(struct deviceapi_part_table));
    api->parititions = &mbr_pt_part_table_total_partitions;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)kmalloc(sizeof(struct mbr_pt_devicedata));
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

void mbr_pt_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}