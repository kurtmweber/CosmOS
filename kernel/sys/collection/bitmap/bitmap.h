
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BITMAP_H
#define _BITMAP_H

#include <types.h>

struct bitmap {
    uint8_t* bits;
    uint64_t size;       // bits
    uint32_t byte_size;  // number of bytes we used
};

struct bitmap* bitmap_new(uint32_t size);
void bitmap_delete(struct bitmap* bm);
void bitmap_set(struct bitmap* bm, uint32_t position, uint8_t value);
uint8_t bitmap_get(struct bitmap* bm, uint32_t position);

#endif