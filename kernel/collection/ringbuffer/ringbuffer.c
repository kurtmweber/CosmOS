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