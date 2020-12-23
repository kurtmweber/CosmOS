
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VIRTIO_H
#define _VIRTIO_H

#include <types.h>

// generic virtio flags
#define VIRTIO_DEVICE_FEATURES      0x00
#define VIRTIO_GUEST_FEATURES       0x04
#define VIRTIO_QUEUE_ADDRESS        0x08
#define VIRTIO_QUEUE_SIZE           0x0C
#define VIRTIO_QUEUE_SELECT         0x0E
#define VIRTIO_QUEUE_NOTIFY         0x10
#define VIRTIO_DEVICE_STATUS        0x12
#define VIRTIO_ISR_STATUS           0x13

// https://wiki.osdev.org/Virtio

#define QUEUE_SIZE  1024
struct Buffers {
    uint64_t Address; // 64-bit address of the buffer on the guest machine.
    uint32_t Length;  // 32-bit length of the buffer.
    uint16_t Flags;   // 1: Next field contains linked buffer index;  2: Buffer is write-only (clear for read-only).
                        // 4: Buffer contains additional buffer addresses.
    uint16_t Next;    // If flag is set, contains index of next buffer in chain.
};

struct Available {
    uint16_t Flags;             // 1: Do not trigger interrupts.
    uint16_t Index;             // Index of the next ring index to be used.  (Last available ring buffer index+1)
    uint16_t Ring[QUEUE_SIZE] ;  // List of available buffer indexes from the Buffers array above.
    uint16_t EventIndex;        // Only used if VIRTIO_F_EVENT_IDX was negotiated
};

struct Ring {
    uint32_t Index;  // Index of the used buffer in the Buffers array above.
    uint32_t Length; // Total bytes written to buffer.
};

struct Used {
    uint16_t Flags;            // 1: Do not notify device when buffers are added to available ring.
    uint16_t Index;            // Index of the next ring index to be used.  (Last used ring buffer index+1)
    struct Ring ring[QUEUE_SIZE];
    uint16_t AvailEvent;       // Only used if VIRTIO_F_EVENT_IDX was negotiated
};

struct VirtualQueue {
    struct Buffers buffers[QUEUE_SIZE]; 
    struct Available available;

    uint8_t Padding[0];  // Reserved
    // 4096 byte alignment
    struct Used used;
};

#endif