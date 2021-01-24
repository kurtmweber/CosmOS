//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/GPT

#include <dev/fs/block_util.h>
#include <dev/partition/partition.h>
#include <dev/partition_table/guid_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_part_table.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

#define GUID_PT_HEADER_LBA 1

const uint8_t GUID_PT_EFI_PART[] = {0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54};

uint8_t guid_pt_part_table_total_partitions(struct device* dev);
uint64_t guid_pt_part_table_get_partition_lba(struct device* dev, uint8_t partition);
uint64_t guid_part_table_get_sector_count_function(struct device* dev, uint8_t partition);

struct guid_pt_devicedata {
    struct device* block_device;
    uint32_t num_partitions;
} __attribute__((packed));

void guid_pt_read_guid_pt_header(struct device* dev, struct guid_pt_header* header) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(header);
    uint16_t buffer_size = block_get_sector_size(dev);
    uint8_t buffer[buffer_size];
    block_read(dev, GUID_PT_HEADER_LBA, buffer);
    memcpy((uint8_t*)header, buffer, sizeof(struct guid_pt_header));
}

/*
 * perform device instance specific init here
 */
void guid_pt_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);

    deviceData->num_partitions = guid_pt_part_table_total_partitions(dev);
    /*
     * mount partition devices
     */
    for (uint32_t i = 0; i < deviceData->num_partitions; i++) {
        partition_attach(deviceData->block_device, guid_pt_part_table_get_partition_lba(dev, i),
                         guid_part_table_get_sector_count_function(dev, i));
    }
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void guid_pt_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
    /*
     * unmount partitions
     */

    /*
     * done w device
     */

    kfree(dev->api);
    kfree(dev->deviceData);
}

uint64_t guid_part_table_get_sector_count_function(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    ASSERT(partition < deviceData->num_partitions);

    struct guid_pt_header header;
    guid_pt_read_guid_pt_header(deviceData->block_device, &header);

    return 0;
}

uint64_t guid_pt_part_table_get_partition_lba(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    ASSERT(partition < deviceData->num_partitions);

    struct guid_pt_header header;
    guid_pt_read_guid_pt_header(deviceData->block_device, &header);

    return 0;
}

uint64_t guid_pt_part_table_get_partition_type(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    ASSERT(partition < deviceData->num_partitions);

    struct guid_pt_header header;
    guid_pt_read_guid_pt_header(deviceData->block_device, &header);
    return 0;
}

uint8_t guid_pt_part_table_total_partitions(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    struct guid_pt_header header;
    guid_pt_read_guid_pt_header(deviceData->block_device, &header);

    return header.num_partitions;
}

uint8_t guid_part_table_attachable(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct guid_pt_header header;
    guid_pt_read_guid_pt_header(dev, &header);
    if (header.magic[0] == GUID_PT_EFI_PART[0]) {
        return 0;
    }
    if (header.magic[1] == GUID_PT_EFI_PART[1]) {
        return 0;
    }
    if (header.magic[2] == GUID_PT_EFI_PART[2]) {
        return 0;
    }
    if (header.magic[3] == GUID_PT_EFI_PART[3]) {
        return 0;
    }
    if (header.magic[4] == GUID_PT_EFI_PART[4]) {
        return 0;
    }
    if (header.magic[5] == GUID_PT_EFI_PART[5]) {
        return 0;
    }
    if (header.magic[6] == GUID_PT_EFI_PART[6]) {
        return 0;
    }
    if (header.magic[7] == GUID_PT_EFI_PART[7]) {
        return 0;
    }
    return 1;
}

uint8_t guid_part_table_detachable(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    // check partitions TODO
}

struct device* guid_pt_attach(struct device* block_device) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &guid_pt_init;
    deviceinstance->uninit = &guid_pt_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARTITION_TABLE;
    devicemgr_set_device_description(deviceinstance, "GUID parition table");
    /*
     * the device api
     */
    struct deviceapi_part_table* api = (struct deviceapi_part_table*)kmalloc(sizeof(struct deviceapi_part_table));
    api->partitions = &guid_pt_part_table_total_partitions;
    api->lba = &guid_pt_part_table_get_partition_lba;
    api->type = &guid_pt_part_table_get_partition_type;
    api->sectors = &guid_part_table_get_sector_count_function;
    api->attachable = &guid_part_table_attachable;
    api->detachable = &guid_part_table_detachable;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)kmalloc(sizeof(struct guid_pt_devicedata));
    deviceData->block_device = block_device;
    deviceData->num_partitions = 0;
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