//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/array/array.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>

struct array* array_new(uint32_t size) {
    struct array* ret = (struct array*)kmalloc(sizeof(struct array));
    ret->byte_size = sizeof(void*) * size;
    ret->data = (void**)kmalloc(ret->byte_size);
    ret->size = size;
    for (uint32_t i = 0; i < size; i++) {
        ret->data[i] = 0;
    }
    return ret;
}

void array_delete(struct array* arr) {
    ASSERT_NOT_NULL(arr);
    if (0 != arr->data) {
        kfree(arr->data);
    }
    kfree(arr);
}

uint32_t array_size(struct array* arr) {
    ASSERT_NOT_NULL(arr);
    return arr->size;
}

void array_set(struct array* arr, uint32_t position, void* value) {
    ASSERT_NOT_NULL(arr);
    if ((position >= 0) && (position < arr->size)) {
        arr->data[position] = value;
    } else {
        kprintf("Index %llu", position);
        panic("invalid array index passed to array_set\n");
    }
}

void* array_get(struct array* arr, uint32_t position) {
    ASSERT_NOT_NULL(arr);
    if ((position >= 0) && (position < arr->size)) {
        return arr->data[position];
    } else {
        kprintf("Index %llu", position);
        panic("invalid array index passed to array_get\n");
    }
}

void array_resize(struct array* arr, uint32_t size) {
    ASSERT_NOT_NULL(arr);
    if (size >= arr->size) {
        /*
         * TODO.  Should use krealloc here, but I think there is a bug in krealloc
         */
        uint32_t oldsize = arr->size;
        void** oldata = arr->data;
        arr->byte_size = sizeof(void*) * size;
        void** newdata = (void**)kmalloc(arr->byte_size);
        for (uint32_t i = 0; i < oldsize; i++) {
            newdata[i] = oldata[i];
        }
        kfree(oldata);
        arr->data = newdata;
        arr->size = size;
    } else {
        panic("arrays cannot be shrunk\n");
    }
}

void array_grow(struct array* arr, uint32_t increment) {
    ASSERT_NOT_NULL(arr);
    array_resize(arr, increment + (arr->size));
}

void array_iterate(struct array* arr, arrayIterator iter) {
    ASSERT_NOT_NULL(arr);
    ASSERT_NOT_NULL(iter);

    for (uint32_t i = 0; i < arr->size; i++) {
        (*iter)(arr->data[i]);
    }
}
