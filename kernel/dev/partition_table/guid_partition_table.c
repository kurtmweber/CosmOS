//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/GPT

#include <dev/fs/block_util.h>
#include <dev/fs/fs_util.h>
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
uint8_t guid_pt_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;

    struct guid_pt_header header;
    guid_pt_read_guid_pt_header(deviceData->block_device, &header);
    if (header.magic[0] != GUID_PT_EFI_PART[0]) {
        return 0;
    }
    if (header.magic[1] != GUID_PT_EFI_PART[1]) {
        return 0;
    }
    if (header.magic[2] != GUID_PT_EFI_PART[2]) {
        return 0;
    }
    if (header.magic[3] != GUID_PT_EFI_PART[3]) {
        return 0;
    }
    if (header.magic[4] != GUID_PT_EFI_PART[4]) {
        return 0;
    }
    if (header.magic[5] != GUID_PT_EFI_PART[5]) {
        return 0;
    }
    if (header.magic[6] != GUID_PT_EFI_PART[6]) {
        return 0;
    }
    if (header.magic[7] != GUID_PT_EFI_PART[7]) {
        return 0;
    }
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);

    // attach partitions
    fsutil_attach_partitions(dev);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t guid_pt_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    // struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    // kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->block_device->name, dev->name);
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

/*
* read a specific parition table entry
*/
void guid_pt_read_guid_pt_entry(struct device* dev, struct guid_pt_entry* entry, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(entry);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;

    /*
    * read the GPT header
    */
    //   struct guid_pt_header header;
    //   memzero((uint8_t*)&header, sizeof(struct guid_pt_header));
    //   guid_pt_read_guid_pt_header(deviceData->block_device, &header);

    // figure out the byte index from the start of the entries
    //    uint32_t idx = partition * sizeof(struct guid_pt_entry);
}

uint64_t guid_part_table_get_sector_count_function(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;

    /*
    * read the partition entry
    */
    struct guid_pt_entry entry;
    memzero((uint8_t*)&entry, sizeof(struct guid_pt_entry));
    guid_pt_read_guid_pt_entry(deviceData->block_device, &entry, partition);

    return 0;
}

uint64_t guid_pt_part_table_get_partition_lba(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;

    /*
    * read the partition entry
    */
    struct guid_pt_entry entry;
    memzero((uint8_t*)&entry, sizeof(struct guid_pt_entry));
    guid_pt_read_guid_pt_entry(deviceData->block_device, &entry, partition);

    return 0;
}

uint64_t guid_pt_part_table_get_partition_type(struct device* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;

    /*
    * read the partition entry
    */
    struct guid_pt_entry entry;
    memzero((uint8_t*)&entry, sizeof(struct guid_pt_entry));
    guid_pt_read_guid_pt_entry(deviceData->block_device, &entry, partition);

    return 0;
}

uint8_t guid_pt_part_table_total_partitions(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;

    /*
    * read the GPT header
    */
    struct guid_pt_header header;
    memzero((uint8_t*)&header, sizeof(struct guid_pt_header));
    guid_pt_read_guid_pt_header(deviceData->block_device, &header);

    /*
    * return result
    */
    return header.num_partitions;
}

uint8_t guid_part_table_detachable(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    //   struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    return 1;
}

struct device* guid_pt_attach(struct device* block_device) {
    ASSERT_NOT_NULL(block_device);
    ASSERT((block_device->devicetype == DISK) || (block_device->devicetype == RAMDISK));
    ASSERT(sizeof(struct guid_pt_entry) == 128);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &guid_pt_init;
    deviceinstance->uninit = &guid_pt_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARTITION_TABLE;
    devicemgr_set_device_description(deviceinstance, "GUID partition table");
    /*
     * the device api
     */
    struct deviceapi_part_table* api = (struct deviceapi_part_table*)kmalloc(sizeof(struct deviceapi_part_table));
    memzero((uint8_t*)api, sizeof(struct deviceapi_part_table));
    api->partitions = &guid_pt_part_table_total_partitions;
    api->lba = &guid_pt_part_table_get_partition_lba;
    api->type = &guid_pt_part_table_get_partition_type;
    api->sectors = &guid_part_table_get_sector_count_function;
    api->detachable = &guid_part_table_detachable;
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

void guid_pt_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}

void guid_pt_dump(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct guid_pt_devicedata* deviceData = (struct guid_pt_devicedata*)dev->deviceData;
    /*
    * read the GPT header
    */
    struct guid_pt_header header;
    memzero((uint8_t*)&header, sizeof(struct guid_pt_header));
    guid_pt_read_guid_pt_header(deviceData->block_device, &header);
    /*
    * start the dump
    */
    kprintf("   ");
    for (uint8_t i = 0; i < 8; i++) {
        kprintf("%#llX ", header.magic[i]);
    }
    kprintf("\n");
    kprintf("   gpt_revision %llu\n", header.gpt_revision);
    kprintf("   header_size %llu\n", header.header_size);
    kprintf("   gpt_lba %llu\n", header.gpt_lba);
    kprintf("   alt_gpt_lba %llu\n", header.alt_gpt_lba);
    kprintf("   first_block %llu\n", header.first_block);
    kprintf("   last_block %llu\n", header.last_block);
    kprintf("   gpt_array_lba %llu\n", header.gpt_array_lba);
    kprintf("   num_partitions %llu\n", header.num_partitions);
    kprintf("   entry_size %llu\n", header.entry_size);
}