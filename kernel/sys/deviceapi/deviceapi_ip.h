//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all IP devices will implement
 */
#ifndef _DEVICEAPI_IP_H
#define _DEVICEAPI_IP_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

struct ip_header {
    uint8_t version : 4;
    uint8_t ihl : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags : 3;
    uint16_t frag_offset : 13;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
} __attribute__((packed));

typedef void (*ip_read_function)(struct device* dev, uint8_t* data, uint16_t size);
typedef void (*ip_write_function)(struct device* dev, uint8_t* data, uint16_t size);

struct deviceapi_ip {
    ip_read_function read;
    ip_write_function write;
};

#endif