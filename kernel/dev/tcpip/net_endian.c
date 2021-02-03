
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/tcpip/net_endian.h>

uint16_t switch_endian16(uint16_t nb) {
    return (nb >> 8) | (nb << 8);
}

uint32_t switch_endian32(uint32_t nb) {
    return ((nb >> 24) & 0xff) | ((nb << 8) & 0xff0000) | ((nb >> 8) & 0xff00) | ((nb << 24) & 0xff000000);
}
