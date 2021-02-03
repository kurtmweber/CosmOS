
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/bitmap/bitmap.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>

struct bitmap* bitmap_new(uint32_t size) {
    struct bitmap* ret = (struct bitmap*)kmalloc(sizeof(struct bitmap));
    ret->byte_size = (size / 8);
    if (ret->byte_size % 8 != 0) {
        ret->byte_size = ret->byte_size + 1;
    }
    //    kprintf("byte size: %llu\n", ret->byte_size);
    ret->bits = kmalloc(ret->byte_size * (sizeof(uint8_t)));
    for (uint16_t i = 0; i < ret->byte_size; i++) {
        ret->bits[i] = 0;
    }
    ret->size = size;
    return ret;
}
void bitmap_delete(struct bitmap* bm) {
    ASSERT_NOT_NULL(bm);
    kfree(bm->bits);
    kfree(bm);
}

void bit_index(uint64_t idx, uint64_t* byte, uint64_t* bit) {
    *byte = idx / 8;
    *bit = idx - (*byte * 8);
}

void bitmap_set(struct bitmap* bm, uint32_t position, uint8_t value) {
    ASSERT_NOT_NULL(bm);
    ASSERT(position < bm->size);

    uint64_t byte = 0;
    uint64_t bit = 0;
    bit_index(position, &byte, &bit);
    uint8_t b = bm->bits[byte];
    if (value > 0) {
        // set
        b = b | (1 << bit);
    } else {
        // clear
        b = b & ~(1 << bit);
    }
    bm->bits[byte] = b;
}

uint8_t bitmap_get(struct bitmap* bm, uint32_t position) {
    ASSERT_NOT_NULL(bm);
    ASSERT(position < bm->size);
    uint64_t byte = 0;
    uint64_t bit = 0;
    bit_index(position, &byte, &bit);
    uint8_t b = bm->bits[byte];

    if (b & (1 << bit)) {
        return 1;
    } else {
        return 0;
    }
}