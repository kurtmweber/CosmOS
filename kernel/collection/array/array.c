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

struct array* array_new(uint32_t size) {
    struct array*  ret = (struct array*) kmalloc(sizeof(struct array));
    ret->data = (void**) kmalloc(sizeof(void*)*size); 
  //  kprintf("array data pointer: %llX\n", (uint64_t)ret->data);
    ret->size = size;
    for (uint32_t i=0; i<size;i++){
        ret->data[i]=0;
    }
    return ret;
}

void array_delete(struct array* arr){
    if (0!=arr){
        if (0!=arr->data){
            kfree(arr->data);
        }
        kfree(arr);
    } else {
        panic("null array passed to array_delete\n");
    }
}

uint32_t array_size(struct array* arr){
    if (0!=arr){
        return arr->size;
    } else {
        panic("null array passed to array_size\n");
    }   
}

void  array_set(struct array* arr, uint32_t position, void* value){
     if (0!=arr){
         if ((position>=0) &&(position< arr->size) ){
             arr->data[position]=value;
         } else {
            kprintf("Index %llu",position);
            panic("invalid array index passed to array_set\n");
        }
     } else {
        panic("null array passed to array_set\n");
    }  
}

void* array_get(struct array* arr,  uint32_t position){
    if (0!=arr){
        if ((position>=0) &&(position< arr->size) ){
            return arr->data[position];
        } else {
            kprintf("Index %llu",position);
            panic("invalid array index passed to array_get\n");
        }  
    } else {
        panic("null array passed to array_get\n");
    }  
}

void array_resize(struct array* arr, uint32_t size) {
    if (0!=arr){
        if (size >= arr->size){
            /*
            * TODO.  Should use krealloc here, but I think there is a bug in krealloc
            */

            uint32_t oldsize = arr->size;
            void** oldata = arr->data;
        //    kprintf("old array data pointer: %llX\n", (uint64_t)oldata);

            void** newdata = (void**) kmalloc(sizeof(void*)*size);

          //  kprintf("new array data pointer: %llX\n", (uint64_t)newdata);

            for (uint32_t i=0;i<oldsize;i++){
                newdata[i]=oldata[i];
            }  
            /*
            * ugh, why does this fail?
            */
            
//            kfree(oldata);
            arr->data = newdata;
            arr->size = size;
        } else {
            panic ("arrays cannot be shrunk\n");
        }
    } else {
        panic("null array passed to array_resize\n");
    }    
}

void array_incrementallyResize(struct array* arr, uint32_t increment) {
    if (0!=arr){
        array_resize(arr, increment+(arr->size));
    } else {
        panic("null array passed to array_incrementallyResize\n");
    }   
}

void array_iterate(struct array* arr, arrayIterator iter) {
    if (0!=iter){
        if (0!=arr){
            for (uint32_t i=0; i<arr->size;i++) {
                (*iter)(arr->data[i]);
            }
        } else {
            panic("null array passed to array_iterate\n ");
        } 
    } else {
        panic("null iterator passed to arrayIterate\n");
    }  
}


