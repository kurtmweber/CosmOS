//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>
#include <tests/sys/test_malloc.h>

#define TEST_MALLOC_LOOPS 1000
#define TEST_MALLOC_INCREMENT 10

void test_malloc() {
    kprintf("Testing malloc\n");

    for (uint32_t i = 0; i < TEST_MALLOC_LOOPS; i++) {
        uint8_t* x = kmalloc(TEST_MALLOC_INCREMENT * (i + 1));
        ASSERT_NOT_NULL(x);
        kmalloc_block* block = kmalloc_block_from_address(x);
        ASSERT(block->used == true);

        ASSERT(kmalloc_block_valid(block));
        ASSERT(block->len >= (TEST_MALLOC_INCREMENT * (i + 1)));

        //      kprintf("Size asked %llu ,Block: %#hX Block size: %llu\n", TEST_MALLOC_INCREMENT * (i + 1), x, block->len);
        kfree(x);
        ASSERT(block->used == false);
    }
}