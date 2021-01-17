//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>

#define EXPAND_SIZE 64
#define START_SIZE 16

struct arraylist* arraylist_new() {
    struct arraylist* ret = (struct arraylist*)kmalloc(sizeof(struct arraylist));
    ret->arr = array_new(START_SIZE);
    ret->count = 0;
}

void arraylist_delete(struct arraylist* lst) {
    ASSERT_NOT_NULL(lst);
    if (0 == lst->arr) {
        panic("why is the underlying array null?!");
    }
    array_delete(lst->arr);
    kfree(lst);
}

uint32_t arraylist_count(struct arraylist* lst) {
    ASSERT_NOT_NULL(lst);
    return lst->count;
}

uint32_t arraylist_size(struct arraylist* lst) {
    ASSERT_NOT_NULL(lst);
    if (0 == lst->arr) {
        panic("why is the underlying array null?!");
    }
    return lst->arr->size;
}

uint32_t arraylist_add(struct arraylist* lst, void* value) {
    ASSERT_NOT_NULL(lst);
    // sanity check
    if (0 == lst->arr) {
        panic("why is the underlying array null?!");
    }
    if (lst->count > array_size(lst->arr)) {
        panic("oh no! what happened?!");
    }

    // expand the underlying array?
    if (lst->count + 1 == array_size(lst->arr)) {
        array_grow(lst->arr, EXPAND_SIZE);
    }
    // save the data
    array_set(lst->arr, lst->count, value);
    lst->count = lst->count + 1;

    // return the index for later use by arraylist_set, arraylist_get, etc
    return lst->count - 1;
}

/*
 * set value at index
 */
void arraylist_set(struct arraylist* lst, uint32_t position, void* value) {
    ASSERT_NOT_NULL(lst);
    if (0 == lst->arr) {
        panic("why is the underlying array null?!");
    }
    if ((position >= 0) && (position < lst->count)) {
        lst->arr->data[position] = value;
    } else {
        panic("invalid list index passed to arraylist_set\n");
    }
}

void* arraylist_get(struct arraylist* lst, uint32_t position) {
    ASSERT_NOT_NULL(lst);
    if (0 == lst->arr) {
        panic("why is the underlying array null?!");
    }
    if ((position >= 0) && (position < lst->count)) {
        return lst->arr->data[position];
    } else {
        panic("invalid list index passed to arraylist_get\n");
    }
}

void arraylist_iterate(struct arraylist* lst, listIterator iter) {
    ASSERT_NOT_NULL(lst);
    ASSERT_NOT_NULL(iter);
    if (0 == lst->arr) {
        panic("why is the underlying array null?!");
    }
    for (uint16_t i = 0; i < lst->count; i++) {
        (*iter)(lst->arr->data[i]);
    }
}

/*
 * remove element at index
 */
void arraylist_remove(struct arraylist* lst, uint32_t position) {
    ASSERT_NOT_NULL(lst);
    if (0 == lst->arr) {
        panic("why is the underlying array null?!");
    }
    if ((position >= 0) && (position < lst->count)) {
        if (position != lst->count - 1) {
            for (uint32_t i = position; i < lst->count - 1; i++) {
                lst->arr->data[i] = lst->arr->data[i + 1];
            }
        }
        lst->count = lst->count - 1;
    } else {
        panic("invalid list index passed to arraylist_remove\n");
    }
}
