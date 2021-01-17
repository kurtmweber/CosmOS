//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/array/array.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <tests/sys/test_array.h>

#define TEST_ARRAY_SIZE 32

void test_array() {
    kprintf("Testing array\n");

    struct array* arr = array_new(TEST_ARRAY_SIZE);
    ASSERT_NOT_NULL(arr);
    ASSERT_NOT_NULL(arr->data);
    ASSERT(arr->byte_size = TEST_ARRAY_SIZE * 8);
    ASSERT(arr->size == TEST_ARRAY_SIZE);
    for (int i = 0; i < TEST_ARRAY_SIZE; i++) {
        ASSERT(0 == array_get(arr, i));
    }

    array_delete(arr);
}