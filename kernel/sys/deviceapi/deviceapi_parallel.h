//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all parallel port devices will implement
 */
#ifndef _DEVICEAPI_PARALLEL_H
#define _DEVICEAPI_PARALLEL_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef void (*parallel_write_function)(struct device* dev, uint8_t* data, uint16_t size);

struct deviceapi_parallel {
    parallel_write_function write;
};

#endif