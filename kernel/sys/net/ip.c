
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://www.saminiir.com/lets-code-tcp-ip-stack-2-ipv4-icmpv4/
// https://tools.ietf.org/html/rfc1071

#include <sys/net/ip.h>

uint16_t ip_checksum(uint16_t *addr, int count) {
    uint32_t ret = 0;
    while (count > 1) {
        ret += *addr++;
        count -= 2;
    }
    if (count > 0) {
        ret += *(uint8_t *)addr;
    }
    while (ret >> 16) {
        ret = (ret & 0xffff) + (ret >> 16);
    }
    return ~ret;
}
