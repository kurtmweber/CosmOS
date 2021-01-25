/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KMALLOC_H
#define _KMALLOC_H

#include <types.h>

#define KMALLOC_ALIGN_BYTES 8

/*
 * the single platform-dependent variable
 */
extern void* brk;

typedef struct mem_block {
    uint8_t start_magic[6];
    struct mem_block* prev;
    void* base;
    uint64_t len;
    bool used;
    uint64_t owner;  // ignored for free blocks
    struct mem_block* next;
    uint8_t end_magic[6];
} __attribute__((aligned(8))) mem_block;

typedef mem_block kmalloc_block;

// kmalloc.c
void kfree(void* ptr);
void* kmalloc(uint64_t size);
void kmalloc_init();
void* krealloc(void* ptr, uint64_t size);
uint8_t kmalloc_pointer_valid(void* ptr);
kmalloc_block* kmalloc_block_from_address(void* ptr);
uint8_t kmalloc_block_valid(kmalloc_block* b);

#endif
