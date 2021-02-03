//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ETHERNET_H
#define _ETHERNET_H

#include <sys/deviceapi/deviceapi_ethernet.h>
#include <types.h>
struct device;

struct device* ethernet_attach(struct device* nic_device);
void ethernet_detach(struct device* dev);

// hw_source and hw_dest are 6 bytes
void ethernet_init_eth(struct eth_hdr* eth, uint8_t* hw_source, uint8_t* hw_dest, uint8_t* data, uint16_t payload_size);
#endif