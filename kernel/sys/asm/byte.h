//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef __BYTE_H
#define __BYTE_H

#include <types.h>

#define LOW_OF_WORD(x) (uint16_t)(x & 0x00FF)
#define HIGH_OF_WORD(x) (uint16_t)((x & 0xFF00) >> 8)
#define LOW_OF_DWORD(x) (uint32_t)(x & 0x0000FFFF)
#define HIGH_OF_DWORD(x) (uint32_t)((x & 0xFFFF0000) >> 16)
#define LOW_OF_QWORD(x) (uint64_t)(x & 0x00000000FFFFFFFF)
#define HIGH_OF_QWORD(x) (uint64_t)((x & 0xFFFFFFFF00000000) >> 32)

// x is address, y is alignment
#define IS_ALIGNED(x, y) ((x % y) == 0)

// for checking if we are aligned on a 4096 byte page
#define IS_ALIGNED_4096(x) ((x % 4096) == 0)

#endif