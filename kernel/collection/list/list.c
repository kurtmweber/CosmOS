//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/list/list.h>
#include <mm/mm.h>
#include <panic/panic.h>

#define EXPAND_SIZE 64
#define START_SIZE 16

struct list* list_new() {
    struct list* ret = (struct list*) kmalloc(sizeof(list_t));
    ret->arr = array_new(START_SIZE);
    ret->count=0;
}

void list_delete(struct list* lst) {
    if (0!=lst){
        if (0==lst->arr){
            panic("why is the underlying array null?!");
        }
        array_delete(lst->arr);
        kfree(lst);
    } else {
        panic("null list passed to list_delete\n");
    }
}

uint32_t list_count(struct list* lst) {
     if (0!=lst){
        return lst->count;
    } else {
        panic("null list passed to list_count\n");
    }
}

uint32_t list_size(struct list* lst) {
      if (0!=lst){
        return lst->arr->size;
    } else {
        panic("null list passed to list_size\n");
    }   
}

uint32_t list_add(struct list* lst, void* value) {
     if (0!=lst){
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
    } else {
        panic("null list passed to list_add\n");
    }
}

void  list_set(struct list* lst, uint32_t position, void* value) {
     if (0!=lst){
         if (0==lst->arr){
            panic("why is the underlying array null?!");
         }
         if ((position>=0) &&(position< lst->count) ){
             lst->arr->data[position]=value;
         } else {
            panic("invalid list index passed to list_set\n");
        }
     } else {
        panic("null list passed to list_set\n");
    }  
}

void* list_get(struct list* lst, uint32_t position) {
    if (0!=lst){
         if (0==lst->arr){
            panic("why is the underlying array null?!");
         }
        if ((position>=0) &&(position< lst->count) ){
            return lst->arr->data[position];
        } else {
            panic("invalid list index passed to list_get\n");
        }  
    } else {
        panic("null list passed to list_get\n");
    }  
}

void list_iterate(struct list* lst, listIterator iter) {
    if (0!=iter){
        if (0!=lst){
            for (uint16_t i=0; i<lst->count;i++) {
                (*iter)(lst->arr->data[i]);
            }
        } else {
            panic("null list passed to list_iterate\n");
        }
    } else {
        panic("null iterator passed to list_iterate\n");
    }
}

