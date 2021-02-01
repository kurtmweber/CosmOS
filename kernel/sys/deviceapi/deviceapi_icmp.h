//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all ICMP devices will implement
 */
#ifndef _DEVICEAPI_ICMP_H
#define _DEVICEAPI_ICMP_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef void (*icmp_read_function)(struct device* dev, uint8_t* data, uint16_t size);
typedef void (*icmp_write_function)(struct device* dev, uint8_t* data, uint16_t size);

struct deviceapi_ip {
    icmp_read_function read;
    icmp_write_function write;
};

#endif