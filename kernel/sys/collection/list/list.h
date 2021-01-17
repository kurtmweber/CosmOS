//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _LIST_H
#define _LIST_H

#include <sys/collection/array/array.h>

/**
 *  a simple list based on an array.  The array is automatically expanded as required to contain all the list items
 */
/**
 * an interator function is provded since for loops are notoriously the cause of bugs
 */
/**
 * indices are zero-based
 */
struct list {
    struct array* arr;
    uint32_t count;
};

/*
 * list iterator
 */
typedef void (*listIterator)(void* value);

/*
 * new list
 */
struct list* list_new();
/*
 * delete list
 */
void list_delete(struct list* lst);
/*
 * number items in the list
 */
uint32_t list_count(struct list* lst);
/*
 * size of the underlying array
 */
uint32_t list_size(struct list* lst);
/*
 * returns zero-based index of added item
 */
uint32_t list_add(struct list* lst, void* value);
/*
 * set value at index
 */
void list_set(struct list* lst, uint32_t position, void* value);
/*
 * get value at index
 */
void* list_get(struct list* lst, uint32_t position);
/*
 * iterate
 */
void list_iterate(struct list* lst, listIterator iter);
/*
 * remove element at index
 */
void list_remove(struct list* lst, uint32_t position);

#endif