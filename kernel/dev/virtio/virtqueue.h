//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VIRTQUEUE_H
#define _VIRTQUEUE_H

#include <types.h>

// https://www.redhat.com/en/blog/virtqueues-and-virtio-ring-how-data-travels

struct virtq_descriptor { 
    uint8_t* addr;
    uint32_t len;
    uint16_t flags;
    uint16_t next; // Will explain this one later in the section "Chained descriptors"
};

struct virtq {
    struct virtq_descriptor** descriptors;
};

// virtq
struct virtq* virtq_new();
void virtq_delete(struct virtq* queue);


// descriptors
struct virtq_descriptor* virtq_descriptor_new();
void virtq_descriptor_delete(struct virtq_descriptor* descriptor);

#endif