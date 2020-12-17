
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
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
typedef struct array {
	void** data;
    uint16_t size;
} array_t;

/*
* array iterator
*/
typedef void (*arrayIterator)(void* value);

struct array* arrayNew(uint16_t size);
void arrayDelete(struct array* arr);
void  arraySet(struct array* arr, uint16_t position, void* value);
void* arrayGet(struct array* arr, uint16_t position);
uint16_t arraySize(struct array* arr);
void arrayResize(struct array* arr, uint16_t size);
void arrayIncrementallyResize(struct array* arr, uint16_t increment);
void arrayIterate(struct array* arr, arrayIterator iter);

#endif