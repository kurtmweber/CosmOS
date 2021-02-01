//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ETHERNET_H
#define _ETHERNET_H

#include <types.h>

#define ETHERNET_HW_LEN 6

struct eth_hdr {
    uint8_t dest_hw[ETHERNET_HW_LEN];
    uint8_t source_hw[ETHERNET_HW_LEN];
    uint16_t type;
    uint8_t* data;
} __attribute__((packed));

#endif