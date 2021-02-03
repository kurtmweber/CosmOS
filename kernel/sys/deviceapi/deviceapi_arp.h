//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all ARP devices will implement
 */
#ifndef _DEVICEAPI_ARP_H
#define _DEVICEAPI_ARP_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

#define ARP_HLEN 6
#define ARP_PLEN 4
#define ARP_ETHERNET 0x01
#define ARP_IP 0x0800
#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x0002
#define RARP_REQUEST 0x0003
#define RARP_REPLY 0x0004

struct arp {
    uint16_t htype;                     // Hardware type
    uint16_t ptype;                     // Protocol type
    uint8_t hlen;                       // Hardware address length (Ethernet = 6)
    uint8_t plen;                       // Protocol address length (IPv4 = 4)
    uint16_t opcode;                    // ARP Operation Code
    uint8_t source_hardware[ARP_HLEN];  // Source hardware address
    uint8_t source_protocol[ARP_PLEN];  // Source protocol address
    uint8_t dest_hardware[ARP_HLEN];    // Destination hardware address
    uint8_t dest_protocol[ARP_PLEN];    // Destination protocol address
};

typedef void (*arp_request_function)(struct device* dev, struct arp* request, struct arp* response);

struct deviceapi_arp {
    arp_request_function request;
};

#endif