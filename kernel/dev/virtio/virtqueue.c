//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/virtio/virtqueue.h>
#include <mm/mm.h>
#include <debug/assert.h>

// https://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.pdf

#define VIRTQ_DESC_F_NEXT       0x01   // This marks a buffer as continuing via the next field.
#define VIRTQ_DESC_F_WRITE      0x02   // This marks a buffer as device write-only (otherwise device read-only).
#define VIRTQ_DESC_F_INDIRECT   0x04   // This means the buffer contains a list of buffer descriptors.

// set this flag in the avail queue if we dont want interrupts
#define VIRTQ_AVAIL_F_NO_INTERRUPT 1

// set this flag in the used queue if we dont want interrupts
#define VIRTQ_USED_F_NO_NOTIFY 1

struct virtq* virtq_new() {
    struct virtq* ret = kmalloc(sizeof(struct virtq));
    /*
    *  allocate descriptor array
    */
    for(uint16_t i=0;i<VIRTQUEUE_SIZE;i++){
       (ret->descriptors)[i]= 0;
    }
    /*
    * avail queue
    */
    for(uint16_t i=0;i<VIRTQUEUE_SIZE;i++){
        ret->avail.ring[i]=0;
    }
    ret->avail.flags=0;
    ret->avail.idx=0;
    /*
    * used queue
    */
    for(uint16_t i=0;i<VIRTQUEUE_SIZE;i++){
        ret->used.ring[i].id=0;
        ret->used.ring[i].len=0;
    }
    ret->used.avail_event=0;
    ret->used.flags=0;
    ret->used.idx=0;
    return ret; 
}

void virtq_delete(struct virtq* queue) {
	ASSERT_NOT_NULL(queue, "queue cannot be null");
    /*
    *  descriptor array
    */
    if (0!=queue->descriptors){
        for(uint16_t i=0;i<VIRTQUEUE_SIZE;i++){
            kfree((queue->descriptors)[i]);
        }
    } else {
        panic("descriptor array should not be null in virtq_delete");
    }
    kfree(queue);
}

/**
 * find empty slot in descriptor array
 */
uint16_t find_first_empty_slot(struct virtq* queue) {
	ASSERT_NOT_NULL(queue, "queue cannot be null");
    for (uint16_t i=0;i<VIRTQUEUE_SIZE;i++){
        if (queue->descriptors[i]==0){
                return i;
            }
        }
    return -1;
}

uint32_t virtq_enqueue_descriptor(struct virtq* queue, struct virtq_descriptor* descriptor) {
	ASSERT_NOT_NULL(queue, "queue cannot be null");
    if (0!=descriptor){
        // find a slot in the descriptor table
        uint16_t slot = find_first_empty_slot(queue);
        // put descriptor into  descriptor table
        queue->descriptors[slot]=descriptor;
        // point to the descriptor in the avail ring
        queue->avail.ring[queue->avail.idx]=slot;
        // save index
        uint32_t saved = queue->avail.idx;
        // increment the avail ring
        queue->avail.idx = queue->avail.idx+1;
        if (queue->avail.idx==VIRTQUEUE_SIZE){
            queue->avail.idx=0;
        }
        return saved;
    } else {
        panic("Invalid buffer passed to virtq_enqueue_buffer");
    }
}

/*
* descriptors
*/
struct virtq_descriptor* virtq_descriptor_new(uint8_t* buffer, uint32_t len) {
	ASSERT_NOT_NULL(buffer, "buffer cannot be null");
    struct virtq_descriptor* ret = kmalloc(sizeof(struct virtq_descriptor));
    ret->addr=buffer;
    ret->flags=0;           // set to zero, indicating that notifications are needed
    ret->len=len;
    ret->next=0;            // there is no next
    return ret;
}

void virtq_descriptor_delete(struct virtq_descriptor* descriptor) {
	ASSERT_NOT_NULL(descriptor, "descriptor cannot be null");
    if (0!=descriptor->addr){
        kfree(descriptor->addr);
    } else {
        panic("virtq_descriptor address should not be zero");
    }
    kfree(descriptor);
}
