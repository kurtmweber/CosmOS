//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************


#include <dev/virtio/virtqueue.h>
#include <mm/mm.h>
#include <panic/panic.h>

#define VIRTQUEUE_BUFFER_SIZE_BYTES 1024*2 // 2k
#define VIRTQUEUE_SIZE 256

struct virtq* virtq_new() {
    struct virtq* ret = kmalloc(sizeof(struct virtq));
    /*
    *  descriptor array
    */
    ret->descriptors = kmalloc(sizeof(struct virtq_descriptor*)*VIRTQUEUE_SIZE);
    for(uint16_t i=0;i<VIRTQUEUE_SIZE;i++){
       (ret->descriptors)[i]= virtq_descriptor_new();
    }
    return ret;
}

void virtq_delete(struct virtq* queue) {
    if (0!=queue){
        /*
        *  descriptor array
        */
        if (0!=queue->descriptors){
            for(uint16_t i=0;i<VIRTQUEUE_SIZE;i++){
                kfree((queue->descriptors)[i]);
            }
            kfree(queue->descriptors);
        } else {
            panic("descriptor array should not be null in virtq_delete");
        }
        kfree(queue);
    } else {
        panic("Invalid virtq passed to virtq__delete");
    }
}


struct virtq_descriptor* virtq_descriptor_new() {
    struct virtq_descriptor* ret = kmalloc(sizeof(struct virtq_descriptor));
    ret->addr=kmalloc(sizeof(uint8_t)*VIRTQUEUE_BUFFER_SIZE_BYTES);
    ret->flags=0;
    ret->len=VIRTQUEUE_BUFFER_SIZE_BYTES;
    ret->next=0;
    return ret;
}

void virtq_descriptor_delete(struct virtq_descriptor* descriptor) {
    if (0!=descriptor){
        if (0!=descriptor->addr){
            kfree(descriptor->addr);

        } else {
            panic("virtq_descriptor address should not be zero");
        }
        kfree(descriptor);
    } else {
        panic("Invalid virtq_descriptor passed to virtq_descriptor_delete");
    }
}


