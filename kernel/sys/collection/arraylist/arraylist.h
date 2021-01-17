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
struct arraylist {
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
struct arraylist* arraylist_new();
/*
 * delete list
 */
void arraylist_delete(struct arraylist* lst);
/*
 * number items in the list
 */
uint32_t arraylist_count(struct arraylist* lst);
/*
 * size of the underlying array
 */
uint32_t arraylist_size(struct arraylist* lst);
/*
 * returns zero-based index of added item
 */
uint32_t arraylist_add(struct arraylist* lst, void* value);
/*
 * set value at index
 */
void arraylist_set(struct arraylist* lst, uint32_t position, void* value);
/*
 * get value at index
 */
void* arraylist_get(struct arraylist* lst, uint32_t position);
/*
 * iterate
 */
void arraylist_iterate(struct arraylist* lst, listIterator iter);
/*
 * remove element at index
 */
void arraylist_remove(struct arraylist* lst, uint32_t position);

#endif