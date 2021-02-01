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
// partition type 16 bytes
typedef void (*part_table_get_partition_type_function)(struct device* dev, uint8_t partition, uint8_t* parititon_type);
typedef uint64_t (*part_table_get_sector_count_function)(struct device* dev, uint8_t partition);

// return 1 if we are ok to detach this device
typedef uint8_t (*part_table_detachable_function)(struct device* dev);

struct deviceapi_part_table {
    part_table_total_partitions_function partitions;
    part_table_get_partition_lba_function lba;
    part_table_get_partition_type_function type;
    part_table_get_sector_count_function sectors;
    part_table_detachable_function detachable;
};

#endif