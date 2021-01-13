//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett & Kurt M. Weber                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#include <types.h>

/**
 *  a simple ringbuffer based on an array.
 */
typedef struct ringbuffer {
    void** data;
    uint16_t start;
    uint16_t end;
} ringbuffer_t;

void ringbuffer_add(struct ringbuffer* buffer, void* add);
void* ringbuffer_consume(struct ringbuffer* buffer);
struct ringbuffer* ringbuffer_new();

#endif