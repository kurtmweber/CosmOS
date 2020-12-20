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

struct ringbuffer* ringbuffer_new(uint16_t size);
void ringbuffer_delete(struct ringbuffer* rb);
void ringbuffer_add(struct ringbuffer* rb, uint64_t value);
uint64_t ringbuffer_remove(struct ringbuffer* rb);
uint16_t ringbuffer_available(struct ringbuffer* rb);

#endif