//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#include <collection/array/array.h>

/**
 *  a simple ringbuffer based on an array. 
 */
typedef struct ringbuffer {
	struct array* arr;
    uint16_t head;
    uint16_t tail;
} ringbuffer_t;

struct ringbuffer* ringbufferNew(uint16_t size);
void ringbufferDelete(struct ringbuffer* rb);
void ringbufferAdd(struct ringbuffer* rb, uint64_t value);
uint64_t ringbufferRemove(struct ringbuffer* rb);
uint16_t ringbufferAvailable(struct ringbuffer* rb);

#endif