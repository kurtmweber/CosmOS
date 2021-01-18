//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all partitions will implement
 */
#ifndef _DEVICEAPI_PARTITION_H
#define _DEVICEAPI_PARTITION_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef uint64_t (*partition_lba_function)(struct device* dev);
typedef struct device* (*partition_device_function)(struct device* dev);

struct deviceapi_partition {
    partition_lba_function lba;
    partition_device_function device;
};

#endif