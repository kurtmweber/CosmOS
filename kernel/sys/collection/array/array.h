
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ARRAY_H
#define _ARRAY_H

#include <types.h>

/**
 * a simple fixed-size array with some accessors and very basic protections
 */
/**
 * an interator function is provded since for loops are notoriously the cause of bugs
 */
/**
 * array is zero-indexed
 */
struct array {
    void** data;
    uint32_t size;
    uint32_t byte_size;
};

/*
 * array iterator
 */
typedef void (*arrayIterator)(void* value);

struct array* array_new(uint32_t size);
void array_delete(struct array* arr);
void array_set(struct array* arr, uint32_t position, void* value);
void* array_get(struct array* arr, uint32_t position);
uint32_t array_size(struct array* arr);
void array_resize(struct array* arr, uint32_t size);
void array_grow(struct array* arr, uint32_t increment);
void array_iterate(struct array* arr, arrayIterator iter);

#endif