//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https:// wiki.osdev.org/Network_Stack#Checksums

#ifndef _NET_ENDIAN_H
#define _NET_ENDIAN_H

#include <types.h>

uint16_t switch_endian16(uint16_t nb);
uint32_t switch_endian32(uint32_t nb);

#endif