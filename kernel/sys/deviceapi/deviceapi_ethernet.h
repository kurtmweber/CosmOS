//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all Ethernet devices will implement
 */
#ifndef _DEVICEAPI_ETHERNET_H
#define _DEVICEAPI_ETHERNET_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

#define ETHERNET_HW_LEN 6

struct eth_hdr {
    uint8_t dest_hw[ETHERNET_HW_LEN];
    uint8_t source_hw[ETHERNET_HW_LEN];
    uint16_t type;
    uint8_t* data;
} __attribute__((packed));

typedef void (*ethernet_read_function)(struct device* dev, struct eth_hdr* eth, uint16_t size);
typedef void (*ethernet_write_function)(struct device* dev, struct eth_hdr* eth, uint16_t size);

struct deviceapi_ethernet {
    ethernet_read_function read;
    ethernet_write_function write;
};

#endif