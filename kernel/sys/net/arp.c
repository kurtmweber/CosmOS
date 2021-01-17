//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kmalloc/kmalloc.h>
#include <sys/net/arp.h>
#include <sys/string/mem.h>

struct arp* arp_new(uint16_t opcode) {
    struct arp* ret = (struct arp*)kmalloc(sizeof(struct arp));
    memset((uint8_t*)ret, 0, sizeof(struct arp));
    ret->htype = ARP_ETHERNET;
    ret->ptype = ARP_IP;
    ret->hlen = ARP_HLEN;
    ret->plen = ARP_PLEN;
    ret->opcode = opcode;
    return ret;
}
