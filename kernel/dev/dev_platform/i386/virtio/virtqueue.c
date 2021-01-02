//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/dev_platform/i386/virtio/virtqueue.h>
#include <system/system_platform/i386/mm/mm.h>
#include <debug/assert.h>
#include <system/console/console.h>

// https://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.pdf

#define VIRTQ_DESC_F_NEXT       0x01   // This marks a buffer as continuing via the next field.
#define VIRTQ_DESC_F_WRITE      0x02   // This marks a buffer as device write-only (otherwise device read-only).
#define VIRTQ_DESC_F_INDIRECT   0x04   // This means the buffer contains a list of buffer descriptors.

// set this flag in the avail queue if we dont want interrupts
#define VIRTQ_AVAIL_F_NO_INTERRUPT 1

// set this flag in the used queue if we dont want interrupts
#define VIRTQ_USED_F_NO_NOTIFY 1

/*
* this variable is set up by the MM
*/
void *virtqueue_buf;

/*
* create virtq
*/
struct virtq* virtq_new(uint16_t size) {
//    struct virtq* ret = kmalloc(sizeof(struct virtq));
    struct virtq* ret = (struct virtq*) virtqueue_buf;
 //   kprintf("VirtQ at %#hX\n", ret);
    /*
    * size
    */
    ret->size = size;
    /*
    *  allocate descriptor pointer array
    */
    ret->descriptors = kmalloc(sizeof(struct virtq_descriptor*)*size);
    for(uint16_t i=0;i<size;i++){
       (ret->descriptors)[i]= 0;
    }
    /*
    * avail queue
    */
    ret->avail.ring = kmalloc(sizeof(uint16_t)*size);
    for(uint16_t i=0;i<size;i++){
        ret->avail.ring[i]=0;
    }
    ret->avail.flags=0;
    ret->avail.idx=0;
    /*
    * used queue
    */
    ret->used.ring = kmalloc(sizeof(struct virtq_used_elem)*size); 
    for(uint16_t i=0;i<size;i++){
        ret->used.ring[i].id=0;
        ret->used.ring[i].len=0;
    }
    ret->used.avail_event=0;
    ret->used.flags=0;
    ret->used.idx=0;
    return ret; 
}

/*
* delete virtq
*/
void virtq_delete(struct virtq* queue) {
	ASSERT_NOT_NULL(queue, "queue cannot be null");
    /*
    *  descriptor array
    */
    if (0!=queue->descriptors){
        for(uint16_t i=0;i<queue->size;i++){
            if (0!=(queue->descriptors)[i]) {
                kfree((queue->descriptors)[i]);
            }
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
    for (uint16_t i=0;i<queue->size;i++){
        if (queue->descriptors[i]==0){
                return i;
            }
        }
    return -1;
}

/*
* enqueue descriptor
*/
uint32_t virtq_enqueue_descriptor(struct virtq* queue, struct virtq_descriptor* descriptor) {
	ASSERT_NOT_NULL(queue, "queue cannot be null");
	ASSERT_NOT_NULL(descriptor, "descriptor cannot be null");

    // find a slot in the descriptor table
    uint16_t slot = find_first_empty_slot(queue);
    if (-1==slot) {
        panic("Ran out of virtual queue slots\n");
    }
    // put descriptor into  descriptor table
    queue->descriptors[slot]=descriptor;
    // point to the descriptor in the avail ring
    queue->avail.ring[queue->avail.idx]=slot;
    // save index
    uint32_t saved = queue->avail.idx;
    // increment the avail ring
    queue->avail.idx = queue->avail.idx+1;
    if (queue->avail.idx==queue->size){
        queue->avail.idx=0;
    }
    return saved;
}

/*
* new descriptor
*/
struct virtq_descriptor* virtq_descriptor_new(uint8_t* buffer, uint32_t len, bool writable) {
	ASSERT_NOT_NULL(buffer, "buffer cannot be null");
    struct virtq_descriptor* ret = kmalloc(sizeof(struct virtq_descriptor));
    ret->addr=buffer;
    if (writable){
        ret->flags=VIRTQ_DESC_F_WRITE; 
    } else {
        ret->flags=0; 
    }         
    ret->len=len;
    ret->next=0;            // there is no next
    return ret;
}

/*
* delete descriptor
*/
void virtq_descriptor_delete(struct virtq_descriptor* descriptor) {
	ASSERT_NOT_NULL(descriptor, "descriptor cannot be null");
    if (0!=descriptor->addr){
        kfree(descriptor->addr);
    } else {
        panic("virtq_descriptor address should not be zero");
    }
    kfree(descriptor);
}

/*
* available idx
*/ 
uint16_t virtq_get_available_idx(struct virtq* queue) {
    ASSERT_NOT_NULL(queue, "queue cannot be null");
    return queue->avail.idx;
}

/*
* used idx
*/ 
uint16_t virtq_get_used_idx(struct virtq* queue) {
	ASSERT_NOT_NULL(queue, "queue cannot be null");
    return queue->used.idx;
}
