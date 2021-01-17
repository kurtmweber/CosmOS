//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Address_Resolution_Protocol

#ifndef _ARP_H
#define _ARP_H

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
    uint8_t source_hardware[ARP_HLEN];  // Source hardware address - hlen bytes (see above)
    uint8_t source_protocol[ARP_PLEN];  // Source protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
    uint8_t dest_hardware[ARP_HLEN];    // Destination hardware address - hlen bytes (see above)
    uint8_t dest_protocol[ARP_PLEN];    // Destination protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
};

struct arp* arp_new(uint16_t opcode);

#endif