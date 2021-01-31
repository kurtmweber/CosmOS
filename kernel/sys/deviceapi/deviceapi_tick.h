//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all tick devices will implement
 */
#ifndef _DEVICEAPI_TICK_H
#define _DEVICEAPI_TICK_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef uint64_t (*tick_read_function)(struct device* dev);

struct deviceapi_tick {
    tick_read_function read;
};

#endif