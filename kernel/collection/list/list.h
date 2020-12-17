//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _LIST_H
#define _LIST_H

#include <collection/array/array.h>

/**
 *  a simple list based on an array.  The array is automatically expanded as required to contain all the list items
 */
/**
 * an interator function is provded since for loops are notoriously the cause of bugs
 */
/**
 * indices are zero-based
 */
typedef struct list {
	struct array* arr;
    uint16_t count;
} list_t;

/*
* list iterator
*/
typedef void (*listIterator)(void* value);

struct list* listNew();
void listDelete(struct list* lst);
/*
* number items in the list
*/
uint16_t listCount(struct list* lst);
/*
* size of the underlying array
*/
uint16_t listSize(struct list* lst);
uint16_t listAdd(struct list* lst, void* value);
void  listSet(struct list* lst, uint16_t position, void* value);
void* listGet(struct list* lst, uint16_t position);
void listIterate(struct list* lst, listIterator iter);

#endif