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

struct list* listNew(uint16_t size) {
    struct list* ret = (struct list*) kmalloc(sizeof(list_t));
    ret->arr = arrayNew(size);
    ret->count=0;
}

void listDelete(struct list* lst) {
    if (0!=lst){
        if (0!=lst->arr){
            arrayDelete(lst->arr);
        }
        kfree(lst);
    } else {
        panic("null list\n");
    }
}

uint16_t listCount(struct list* lst) {
     if (0!=lst){
        return lst->count;
    } else {
        panic("null list\n");
    }
}

uint16_t listAdd(struct list* lst, void* value) {
     if (0!=lst){
         // sanity check
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
        panic("null list\n");
    }
}

void  listSet(struct list* lst, uint16_t position, void* value) {
     if (0!=lst){
         if ((position>=0) &&(position< lst->count) ){
             lst->arr->data[position]=value;
         } else {
            panic("invalid list index\n");
        }
     } else {
        panic("null list\n");
    }  
}

void* listGet(struct list* lst, uint16_t position) {
    if (0!=lst){
        if ((position>=0) &&(position< lst->count) ){
            return lst->arr->data[position];
        } else {
            panic("invalid list index\n");
        }  
    } else {
        panic("null list\n");
    }  
}


