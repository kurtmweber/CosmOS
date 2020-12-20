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

struct list* listNew() {
    struct list* ret = (struct list*) kmalloc(sizeof(list_t));
    ret->arr = arrayNew(START_SIZE);
    ret->count=0;
}

void listDelete(struct list* lst) {
    if (0!=lst){
        if (0==lst->arr){
            panic("why is the underlying array null?!");
        }
        arrayDelete(lst->arr);
        kfree(lst);
    } else {
        panic("null list passed to listDelete\n");
    }
}

uint16_t listCount(struct list* lst) {
     if (0!=lst){
        return lst->count;
    } else {
        panic("null list passed to listCount\n");
    }
}

uint16_t listSize(struct list* lst) {
      if (0!=lst){
        return lst->arr->size;
    } else {
        panic("null list passed to listSize\n");
    }   
}

void listAdd(struct list* lst, void* value) {
     if (0!=lst){
         // sanity check
         if (0==lst->arr){
            panic("why is the underlying array null?!");
         }
         if (lst->count > arraySize(lst->arr)){
             panic("oh no! what happened?!");
         }
         // expand the underlying array?
         if (lst->count == arraySize(lst->arr)){
             arrayIncrementallyResize(lst->arr, EXPAND_SIZE);
         }
        // save the data
        arraySet(lst->arr, lst->count, value);
        lst->count = lst->count+1;
    } else {
        panic("null list passed to listAdd\n");
    }
}

void  listSet(struct list* lst, uint16_t position, void* value) {
     if (0!=lst){
         if (0==lst->arr){
            panic("why is the underlying array null?!");
         }
         if ((position>=0) &&(position< lst->count) ){
             lst->arr->data[position]=value;
         } else {
            panic("invalid list index passed to listSet\n");
        }
     } else {
        panic("null list passed to listSet\n");
    }  
}

void* listGet(struct list* lst, uint16_t position) {
    if (0!=lst){
         if (0==lst->arr){
            panic("why is the underlying array null?!");
         }
        if ((position>=0) &&(position< lst->count) ){
            return lst->arr->data[position];
        } else {
            panic("invalid list index passed to listGet\n");
        }  
    } else {
        panic("null list passed to listGet\n");
    }  
}

void listIterate(struct list* lst, listIterator iter) {
    if (0!=iter){
        if (0!=lst){
            for (uint16_t i=0; i<lst->count;i++) {
                (*iter)(lst->arr->data[i]);
            }
        } else {
            panic("null list passed to listIterate\n");
        }
    } else {
        panic("null iterator passed to listIterate\n");
    }
}

