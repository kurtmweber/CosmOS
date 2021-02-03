//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/bitmap/bitmap.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/sys/test_bitmap.h>

void test_bitmap() {
    kprintf("Testing bitmap\n");

    struct bitmap* bm = bitmap_new(10);

    ASSERT(bitmap_get(bm, 0) == 0);
    bitmap_set(bm, 0, 1);
    ASSERT(bitmap_get(bm, 0) == 1);
    bitmap_set(bm, 0, 1);
    ASSERT(bitmap_get(bm, 0) == 1);
    bitmap_set(bm, 0, 0);
    ASSERT(bitmap_get(bm, 0) == 0);

    bitmap_delete(bm);
}
