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

struct deviceapi_part_table {
    part_table_total_partitions_function parititions;
};

#endif