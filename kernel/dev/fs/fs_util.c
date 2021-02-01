//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/fat/fat.h>
#include <dev/fs/fs_util.h>
#include <dev/fs/tfs/tfs.h>
#include <dev/partition/partition.h>
#include <dev/partition_table/guid_partition_table.h>
#include <dev/partition_table/mbr_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_part_table.h>
#include <sys/devicemgr/devicemgr.h>

void fsutil_attach_partition_tables(struct device* block_dev) {
    ASSERT_NOT_NULL(block_dev);

    // try to attach gpt
    struct device* gpt = guid_pt_attach(block_dev);
    if (0 == gpt) {
        // maybe its mbr
        mbr_pt_attach(block_dev);
    } else {
        guid_pt_dump(gpt);
    }
}

void fsutil_detach_partition_tables(struct device* block_dev) {
    ASSERT_NOT_NULL(block_dev);
}

void fsutil_attach_partitions(struct device* partition_table_dev) {
    ASSERT_NOT_NULL(partition_table_dev);
    /*
     * mount partition devices
     */
    struct deviceapi_part_table* api = (struct deviceapi_part_table*)partition_table_dev->api;
    uint32_t num_partitions = (*api->partitions)(partition_table_dev);
    for (uint32_t i = 0; i < num_partitions; i++) {
        uint32_t lba = (*api->lba)(partition_table_dev, i);
        uint32_t sector_count = (*api->sectors)(partition_table_dev, i);
        if (sector_count > 0) {
            partition_attach(partition_table_dev, lba, sector_count);
        }
    }
}

void fsutil_detach_partitions(struct device* partition_table_dev) {
    ASSERT_NOT_NULL(partition_table_dev);
}

void fsutil_attach_fs(struct device* partition_dev) {
    if (0 == fat_attach(partition_dev)) {
        tfs_attach(partition_dev);
    }
}

void fsutil_detach_fs(struct device* partition_dev) {
    ASSERT_NOT_NULL(partition_dev);
}
