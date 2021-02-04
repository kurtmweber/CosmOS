//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>

#define EXPAND_SIZE 64
#define START_SIZE 16

struct arraylist* arraylist_new() {
    struct arraylist* ret = (struct arraylist*)kmalloc(sizeof(struct arraylist));
    ret->arr = array_new(START_SIZE);
    ret->count = 0;
    return ret;
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
    return 0;
}

void arraylist_iterate(struct arraylist* lst, list_iterator iter) {
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

/*
* swap values
*/
void arraylist_swap(struct arraylist* lst, uint32_t idx1, uint32_t idx2) {
    ASSERT_NOT_NULL(lst);
    ASSERT_NOT_NULL(lst->arr);
    ASSERT(idx1 < lst->count);
    ASSERT(idx2 < lst->count);

    void* v1 = array_get(lst->arr, idx1);
    void* v2 = array_get(lst->arr, idx2);
    array_set(lst->arr, idx1, v2);
    array_set(lst->arr, idx2, v1);
}

int arraylist_partition(struct arraylist* lst, list_comparator comparator, uint32_t low, uint32_t high) {
    ASSERT_NOT_NULL(lst);
    ASSERT_NOT_NULL(lst->arr);
    ASSERT(low <= lst->count);
    ASSERT(high < lst->count);

    // Select the pivot element
    void* pivot = array_get(lst->arr, high);
    uint32_t i = (low - 1);

    // Put the elements smaller than pivot on the left
    // and greater than pivot on the right of pivot
    for (uint32_t j = low; j < high; j++) {
        void* this_element = array_get(lst->arr, j);
        if ((*comparator)(pivot, this_element)) {
            i++;
            arraylist_swap(lst, i, j);
        }
    }

    arraylist_swap(lst, i + 1, high);
    return (i + 1);
}

void arraylist_sort_internal(struct arraylist* lst, list_comparator comparator, uint32_t low, uint32_t high) {
    ASSERT(low <= lst->count);
    ASSERT(high < lst->count);

    if (low < high) {
        // Select pivot position and put all the elements smaller
        // than pivot on left and greater than pivot on right
        int pi = arraylist_partition(lst, comparator, low, high);

        // Sort the elements on the left of pivot
        if (pi > 0) {
            arraylist_sort_internal(lst, comparator, low, pi - 1);
        }

        // Sort the elements on the right of pivot
        arraylist_sort_internal(lst, comparator, pi + 1, high);
    }
}

/*
* quicksort
*/
void arraylist_sort(struct arraylist* lst, list_comparator comparator) {
    ASSERT_NOT_NULL(lst);
    ASSERT_NOT_NULL(comparator);
    if ((lst->arr != 0) && (lst->count > 1)) {
        arraylist_sort_internal(lst, comparator, 0, lst->count - 1);
    }
}

/*
* a comparator for strings
*/
uint8_t arraylist_string_comparator(void* e1, void* e2) {
    ASSERT_NOT_NULL(e1);
    ASSERT_NOT_NULL(e2);
    if (strcmp((uint8_t*)e1, (uint8_t*)e2) == 1) {
        return 1;
    }
    return 0;
}
