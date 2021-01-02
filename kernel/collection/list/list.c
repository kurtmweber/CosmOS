//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/list/list.h>
#include <system/platform/i386/mm/mm.h>
#include <debug/assert.h>

#define EXPAND_SIZE 64
#define START_SIZE 16

struct list* list_new() {
    struct list* ret = (struct list*) kmalloc(sizeof(struct list));
    ret->arr = array_new(START_SIZE);
    ret->count=0;
}

void list_delete(struct list* lst) {
	ASSERT_NOT_NULL(lst, "list cannot be null");
    if (0==lst->arr){
        panic("why is the underlying array null?!");
    }
    array_delete(lst->arr);
    kfree(lst);
}

uint32_t list_count(struct list* lst) {
	ASSERT_NOT_NULL(lst, "list cannot be null");
    return lst->count;
}

uint32_t list_size(struct list* lst) {
	ASSERT_NOT_NULL(lst, "list cannot be null");
    return lst->arr->size;
}

uint32_t list_add(struct list* lst, void* value) {
	ASSERT_NOT_NULL(lst, "list cannot be null");
    // sanity check
    if (0==lst->arr){
        panic("why is the underlying array null?!");
    }
    if (lst->count > array_size(lst->arr)){
        panic("oh no! what happened?!");
    }

    // expand the underlying array?
    if (lst->count+1 == array_size(lst->arr)){
        array_incrementallyResize(lst->arr, EXPAND_SIZE);
    }
    // save the data
    array_set(lst->arr, lst->count, value);
    lst->count = lst->count+1;
    return lst->count-1;
}

void  list_set(struct list* lst, uint32_t position, void* value) {
	ASSERT_NOT_NULL(lst, "list cannot be null");
    if (0==lst->arr){
        panic("why is the underlying array null?!");
    }
    if ((position>=0) &&(position< lst->count) ){
        lst->arr->data[position]=value;
    } else {
        panic("invalid list index passed to list_set\n");
    }
}

void* list_get(struct list* lst, uint32_t position) {
	ASSERT_NOT_NULL(lst, "list cannot be null");
    if (0==lst->arr){
        panic("why is the underlying array null?!");
        }
    if ((position>=0) &&(position< lst->count) ){
        return lst->arr->data[position];
    } else {
        panic("invalid list index passed to list_get\n");
    }  
}

void list_iterate(struct list* lst, listIterator iter) {
	ASSERT_NOT_NULL(lst, "list cannot be null");
	ASSERT_NOT_NULL(iter, "iter cannot be null");
    for (uint16_t i=0; i<lst->count;i++) {
        (*iter)(lst->arr->data[i]);
    }
}

