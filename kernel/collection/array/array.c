//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/array/array.h>

#include <mm/mm.h>
#include <panic/panic.h>

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
        panic("null array\n");
    }
}

uint16_t arraySize(struct array* arr){
    if (0!=arr){
        return arr->size;
    } else {
        panic("null array\n");
    }   
}

void  arraySet(struct array* arr, uint16_t position, void* value){
     if (0!=arr){
         if ((position>=0) &&(position< arr->size) ){
             arr->data[position]=value;
         } else {
            panic("invalid array index\n");
        }
     } else {
        panic("null array\n");
    }  
}

void* arrayGet(struct array* arr,  uint16_t position){
    if (0!=arr){
        if ((position>=0) &&(position< arr->size) ){
            return arr->data[position];
        } else {
            panic("invalid array index\n");
        }  
    } else {
        panic("null array\n");
    }  
}

void arrayResize(struct array* arr, uint16_t size) {
    if (0!=arr){
        if (size >= arr->size){
            arr = krealloc(arr, size);
            arr->size = size;
        } else {
            ("arrays cannot be shrunk\n");
        }
    } else {
        panic("null array\n");
    }    
}

void arrayIncrementallyResize(struct array* arr, uint16_t increment) {
    if (0!=arr){
        arrayResize(arr, increment+arr->size);
    } else {
        panic("null array\n");
    }   
}


