//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Partition_Table
// https://wiki.osdev.org/MBR_(x86)
// https://en.wikipedia.org/wiki/Master_boot_record

#include <dev/fs/block_util.h>
#include <dev/fs/fs_util.h>
#include <dev/partition/partition.h>
#include <dev/partition_table/mbr_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_part_table.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

#define MBR_HEADER_LBA 0

uint8_t mbr_pt_part_table_total_partitions(struct device* dev);
uint64_t mbr_pt_part_table_get_partition_lba(struct device* dev, uint8_t partition);
uint64_t mbr_part_table_get_sector_count_function(struct device* dev, uint8_t partition);

struct mbr_pt_devicedata {
    struct device* block_device;
    uint32_t num_partitions;
} __attribute__((packed));

void mbr_pt_read_mbr_pt_header(struct device* dev, struct mbr_pt_header* header) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(header);
    uint16_t buffer_size = block_get_sector_size(dev);
    uint8_t buffer[buffer_size];
    block_read(dev, MBR_HEADER_LBA, buffer);
    memcpy((uint8_t*)header, buffer, sizeof(struct mbr_pt_header));
}

/*
 * perform device instance specific init here
 */
uint8_t mbr_pt_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;

    deviceData->num_partitions = mbr_pt_part_table_total_partitions(dev);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(deviceData->block_device, &header);
    if ((header.signature[0] == 0x55) && (header.signature[1] == 0xAA)) {
        kprintf("Init %s on %s (%s) with %llu partitions\n", dev->description, deviceData->block_device->name,
                dev->name, deviceData->num_partitions);

        // attach partitions
        fsutil_attach_partitions(dev);

        return 1;
    }
    return 0;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t mbr_pt_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);

    /*
     * unmount partitions
     */
    fsutil_detach_partitions(dev);

    /*
     * done w device
     */
    kfree(dev->api);
    kfree(dev->deviceData);
    return 1;
}

uint64_t mbr_pt_part_table_get_partition_lba(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    ASSERT(partition < deviceData->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(deviceData->block_device, &header);
    return header.partitions[partition].lba_start;
}

uint64_t mbr_part_table_get_sector_count_function(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    ASSERT(partition < deviceData->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(deviceData->block_device, &header);
    return header.partitions[partition].total_sectors;
}

void mbr_pt_part_table_get_partition_type(struct device* dev, uint8_t partition, uint8_t* parititon_type,
                                          uint16_t len) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    ASSERT_NOT_NULL(parititon_type);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    ASSERT(partition < deviceData->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(deviceData->block_device, &header);
    uitoa3(header.partitions[partition].partition_type, parititon_type, len, 16);
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

uint8_t mbr_part_table_detachable(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;

    // check the partitions TODO
    return 1;
}

void mbr_part_read_sector(struct device* dev, uint8_t partition_index, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    uint64_t lba = mbr_pt_part_table_get_partition_lba(dev, partition_index);
    block_read_sectors(deviceData->block_device, lba + sector, data, count);
}

void mbr_part_write_sector(struct device* dev, uint8_t partition_index, uint32_t sector, uint8_t* data,
                           uint32_t count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    uint64_t lba = mbr_pt_part_table_get_partition_lba(dev, partition_index);
    block_write_sectors(deviceData->block_device, lba + sector, data, count);
}

uint16_t mbr_part_sector_size(struct device* dev, uint8_t partition_index) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    panic("not implemented");
    return 0;
}

uint32_t mbr_part_total_size(struct device* dev, uint8_t partition_index) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)dev->deviceData;
    panic("not implemented");
    return 0;
}

struct device* mbr_pt_attach(struct device* block_device) {
    ASSERT(sizeof(struct mbr_pt_entry) == 16);
    ASSERT_NOT_NULL(block_device);
    ASSERT((block_device->devicetype == DISK) || (block_device->devicetype == RAMDISK) ||
           (block_device->devicetype == VBLOCK));

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &mbr_pt_init;
    deviceinstance->uninit = &mbr_pt_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARTITION_TABLE;
    devicemgr_set_device_description(deviceinstance, "MBR partition table");
    /*
     * the device api
     */
    struct deviceapi_part_table* api = (struct deviceapi_part_table*)kmalloc(sizeof(struct deviceapi_part_table));
    memzero((uint8_t*)api, sizeof(struct deviceapi_part_table));
    api->partitions = &mbr_pt_part_table_total_partitions;
    api->lba = &mbr_pt_part_table_get_partition_lba;
    api->type = &mbr_pt_part_table_get_partition_type;
    api->sectors = &mbr_part_table_get_sector_count_function;
    api->detachable = &mbr_part_table_detachable;
    api->read = &mbr_part_read_sector;
    api->write = &mbr_part_write_sector;
    api->sector_size = &mbr_part_sector_size;
    api->total_size = &mbr_part_total_size;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct mbr_pt_devicedata* deviceData = (struct mbr_pt_devicedata*)kmalloc(sizeof(struct mbr_pt_devicedata));
    deviceData->block_device = block_device;
    deviceData->num_partitions = 0;
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
        kfree(deviceData);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void mbr_pt_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}