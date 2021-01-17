//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all swap devices will implement
 */
#ifndef _DEVICEAPI_SWAP_H
#define _DEVICEAPI_SWAP_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef void (*swap_read_function)(struct device* dev, uint8_t* data, uint32_t block);
typedef void (*swap_write_function)(struct device* dev, uint8_t* data, uint32_t block);
typedef void (*swap_mount_function)(struct device* dev);

struct deviceapi_swap {
    swap_read_function read;
    swap_write_function write;
    swap_mount_function mount;
};

#endif