//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <collection/ringbuffer/ringbuffer.h>
#include <mm/mm.h>
#include <panic/panic.h>

#define RINGBUFFER_SIZE 256

void ringbuffer_add(struct ringbuffer* buffer, uint64_t add) {
    if (0!=buffer){
 
        buffer->data[buffer->end] = add;
	
        if (buffer->end == RINGBUFFER_SIZE){
            buffer->end = 0;
        } else {
            buffer->end++;
        }
        
        if (buffer->end == buffer->start){
            if (buffer->start == RINGBUFFER_SIZE){
                buffer->start = 0;
            } else {
                buffer->start++;
            }
        }	
    } else {
        panic("null buffer passed to ringbuffer_add\n");
    }
}

uint64_t ringbuffer_consume(struct ringbuffer* buffer) {
   if (0!=buffer){
        uint16_t i;
        
        if ((buffer->end == 0) && (buffer->start == 0)){
            return 0;
        }
        
        i = buffer->start;	// we need this so we can return keyboard_buffer[i] in case we reset keyboard_buffer_start and keyboard_buffer_end to 0 if we reach the end
        
        if (buffer->start == RINGBUFFER_SIZE){
            buffer->start = 0;
        } else {
            buffer->start++;
        }
        
        if (buffer->start == buffer->end){
            buffer->start = 0;
            buffer->end = 0;
        }
        return buffer->data[i];
    } else {
        panic("null buffer passed to ringbuffer_consume\n");        
    }
}

struct ringbuffer* ringbuffer_new() {
    struct ringbuffer* ret = (struct ringbuffer*) kmalloc(sizeof(struct ringbuffer));
    ret->start = 0;
    ret->end=0;
    ret->data = kmalloc(sizeof(uint64_t)*RINGBUFFER_SIZE);
    return ret;
}

