
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DYNARRAY_H
#define _DYNARRAY_H

#include <types.h>

/**
 * a dynamic array with some accessors and very basic protections
 */
/**
 * array is zero-indexed
 */
typedef struct array {
	void** data;
    uint16_t size;
} array_t;

struct array* dynArrayNew(uint16_t size);
void dynArrayDelete(struct array* arr);
void  dynArraySet(struct array* arr, uint16_t position, void* value);
void* dynArrayGet(struct array* arr, uint16_t position);
uint16_t dynArraySize(struct array* arr);

#endif