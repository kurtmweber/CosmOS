//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all PIT devices will implement
 */
#ifndef _DEVICEAPI_PIT_H
#define _DEVICEAPI_PIT_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef void (*pit_event)();
typedef uint64_t (*pit_tickcount_function)(struct device* dev);
typedef void (*pit_subscribe_function)(pit_event event);

struct deviceapi_pit {
    pit_tickcount_function tickcount;
    pit_subscribe_function subscribe;
};

#endif