//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <array/array.h>

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
        panic("null array");
    }
}

uint16_t arraySize(struct array* arr){
    if (0!=arr){
        return arr->size;
    } else {
        panic("null array");
    }   
}

void  arraySet(struct array* arr, uint16_t position, void* value){
     if (0!=arr){
         if ((position>=0) &&(position< arr->size) ){
             arr->data[position]=value;
         } else {
            panic("invalid array index");
        }
     } else {
        panic("null array");
    }  
}

void* arrayGet(struct array* arr,  uint16_t position){
    if (0!=arr){
        if ((position>=0) &&(position< arr->size) ){
            return arr->data[position];
        } else {
            panic("invalid array index");
        }  
    } else {
        panic("null array");
    }  
}
