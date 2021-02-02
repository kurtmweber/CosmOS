//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all partition tables (MBR, GUID) will implement
 */
#ifndef _DEVICEAPI_PART_TABLE_H
#define _DEVICEAPI_PART_TABLE_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef uint8_t (*part_table_total_partitions_function)(struct device* dev);
typedef uint64_t (*part_table_get_partition_lba_function)(struct device* dev, uint8_t partition);
// partition type 64 byte string
typedef void (*part_table_get_partition_type_function)(struct device* dev, uint8_t partition, uint8_t* parititon_type,
                                                       uint16_t len);
typedef uint64_t (*part_table_get_sector_count_function)(struct device* dev, uint8_t partition);
typedef void (*part_read_sector_function)(struct device* dev, uint8_t partition_index, uint32_t sector, uint8_t* data,
                                          uint32_t count);
typedef void (*part_write_sector_function)(struct device* dev, uint8_t partition_index, uint32_t sector, uint8_t* data,
                                           uint32_t count);
typedef uint16_t (*part_sector_size_function)(struct device* dev, uint8_t partition_index);
typedef uint32_t (*part_total_size_function)(struct device* dev, uint8_t partition_index);

// return 1 if we are ok to detach this device
typedef uint8_t (*part_table_detachable_function)(struct device* dev);

struct deviceapi_part_table {
    part_table_total_partitions_function partitions;
    part_table_get_partition_lba_function lba;
    part_table_get_partition_type_function type;
    part_table_get_sector_count_function sectors;
    part_table_detachable_function detachable;
    part_read_sector_function read;
    part_write_sector_function write;
    part_sector_size_function sector_size;
    part_total_size_function total_size;
};

#endif