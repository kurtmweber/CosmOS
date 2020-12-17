//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/ringbuffer/ringbuffer.h>
#include <mm/mm.h>
#include <panic/panic.h>


struct ringbuffer* ringbufferNew(uint16_t size) {
    struct ringbuffer* ret = (struct ringbuffer*) kmalloc(sizeof(ringbuffer_t));
    ret->arr = arrayNew(size);
    ret->head=0;
    ret->tail=0;
}

void ringbufferDelete(struct ringbuffer* rb) {
    if (0!=rb){
        if (0==rb->arr){
            panic("why is the underlying array null?!");
        }
        arrayDelete(rb->arr);
        kfree(rb);
    } else {
        panic("null ringbuffer\n");
    }
}

void ringbufferAdd(struct ringbuffer* rb, uint64_t value) {
    if (0!=rb){
        if (0==rb->arr){
            panic("why is the underlying array null?!");
        }
        arraySet(rb->arr, rb->head, (void*) value);
        rb->head=rb->head+1;
        // wrap
        if (rb->head == arraySize(rb->arr)){
            rb->head=0;
        }
    } else {
        panic("null ringbuffer\n");
    }   
}

uint64_t ringbufferRemove(struct ringbuffer* rb) {
    if (0!=rb){
        if (0==rb->arr){
            panic("why is the underlying array null?!");
        }
        if (rb->head > rb->tail) {
            uint64_t ret = (uint64_t) arrayGet(rb->arr, rb->tail);      
            rb->tail=rb->tail+1;
            // wrap
            if (rb->tail == arraySize(rb->arr)){
                rb->tail=0;
            }
        } else {
            panic("ringbuffer underflow");
        }
    } else {
        panic("null ringbuffer\n");
    }   
}

uint16_t ringbufferAvailable(struct ringbuffer* rb) {
     if (0!=rb){
        return (rb->head - rb->tail);
    } else {
        panic("null ringbuffer\n");
    }     
}

