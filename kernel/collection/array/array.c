//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/array/array.h>

#include <mm/mm.h>
#include <panic/panic.h>
#include <console/console.h>

struct array* arrayNew(uint16_t size) {
    struct array*  ret = (struct array*) kmalloc(sizeof(array_t));
    ret->data = (void**) kmalloc(sizeof(void*)*size); 
    ret->size = size;
    for (uint16_t i=0; i<size;i++){
        ret->data[i]=0;
    }
    return ret;
}

void arrayDelete(struct array* arr){
    if (0!=arr){
        if (0!=arr->data){
            kfree(arr->data);
        }
        kfree(arr);
    } else {
        panic("null array passed to arrayDelete\n");
    }
}

uint16_t arraySize(struct array* arr){
    if (0!=arr){
        return arr->size;
    } else {
        panic("null array passed to arraySize\n");
    }   
}

void  arraySet(struct array* arr, uint16_t position, void* value){
     if (0!=arr){
         if ((position>=0) &&(position< arr->size) ){
             arr->data[position]=value;
         } else {
            kprintf("Index %llu",position);
            panic("invalid array index passed to arraySet\n");
        }
     } else {
        panic("null array passed to arraySet\n");
    }  
}

void* arrayGet(struct array* arr,  uint16_t position){
    if (0!=arr){
        if ((position>=0) &&(position< arr->size) ){
            return arr->data[position];
        } else {
            kprintf("Index %llu",position);
            panic("invalid array index passed to arrayGet\n");
        }  
    } else {
        panic("null array passed to arrayGet\n");
    }  
}

void arrayResize(struct array* arr, uint16_t size) {
    if (0!=arr){
        if (size >= arr->size){
            arr->data = krealloc(arr->data, size);
            arr->size = size;         
        } else {
            panic ("arrays cannot be shrunk\n");
        }
    } else {
        panic("null array passed to arrayResize\n");
    }    
}

void arrayIncrementallyResize(struct array* arr, uint16_t increment) {
    if (0!=arr){
        arrayResize(arr, increment+(arr->size));
    } else {
        panic("null array passed to arrayIncrementallyResize\n");
    }   
}

void arrayIterate(struct array* arr, arrayIterator iter) {
    if (0!=iter){
        if (0!=arr){
            for (uint16_t i=0; i<arr->size;i++) {
                (*iter)(arr->data[i]);
            }
        } else {
            panic("null array passed to arrayIterate\n ");
        } 
    } else {
        panic("null iterator passed to arrayIterate\n");
    }  
}


