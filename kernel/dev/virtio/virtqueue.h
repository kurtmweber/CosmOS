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

#define VIRTQ_DESC_F_AVAIL (1 << 7)
#define VIRTQ_DESC_F_USED (1 << 15)

struct virtq_descriptor { 
    uint8_t* addr;
    uint32_t len;
    uint16_t flags;
    uint16_t next;
};

struct virtq_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t* ring;
};

struct virtq_used_elem {
    uint32_t id;    // Index of start of used descriptor chain.
    uint32_t len;   // Total length of the descriptor chain which was used (written to) 
};

struct virtq_used {
    uint16_t flags;
    uint16_t idx;
    struct virtq_used_elem* ring;
    uint16_t avail_event;                           // Only if VIRTIO_F_EVENT_IDX
};

struct virtq {
    uint16_t size;
    struct virtq_descriptor** descriptors;
    struct virtq_avail avail;
    struct virtq_used used;
};

// virtq
struct virtq* virtq_new(uint16_t size);
void virtq_delete(struct virtq* queue);
uint32_t virtq_enqueue_descriptor(struct virtq* queue, struct virtq_descriptor* descriptor);

// descriptors
struct virtq_descriptor* virtq_descriptor_new(uint8_t* buffer, uint32_t len, bool writable);
void virtq_descriptor_delete(struct virtq_descriptor* descriptor);

// available
uint16_t virtq_get_available_idx(struct virtq* queue);

// used
uint16_t virtq_get_used_idx(struct virtq* queue);


#endif