/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KMALLOC_C
#define _KMALLOC_C

#include <types.h>
#include <debug_error/debug_error.h>
#include <mm/mm.h>

kmalloc_block *find_avail_kmalloc_block_list(uint64_t size){
	kmalloc_block *cur_block;
	kmalloc_block *best = 0;
	kmalloc_block *last = 0;
	// Find the first block on the heap suitable for the size requested.
	// If we find an unused block of exactly the size requested, return it immediately.
	// Otherwise, we return the smallest unused block that is AT LEAST big enough to
	// satisfy the request.  For now, we make no attempt to split the block--later on
	// that may be added, but for now let's keep it simple even if that results in a more
	// inefficient use of heap space (running out of address space is at any rate an unlikely
	// issue for 64-bit, though physical space is perhaps a different story)
	
	// If no sufficiently-sized block is found, create a new one after the last block.
	// If there is not enough address space to hold the block (including the header, return NULL
	
	// this shouldn't happen, because kmalloc() should initialize the list the first time it is called	
	if (!kmalloc_block_list){
		panic("Uninitialized kmalloc block list!");
	}
	
	cur_block = kmalloc_block_list;
	
	do {
		if (cur_block->used != false){
			continue;
		}
		
		if (cur_block->len == size){
			return cur_block;
		}
		
		if (cur_block->len > size){
			if (!best){
				best = cur_block;
			} else 	if (best->len < cur_block->len){
				best = cur_block;
			} else {
				continue;
			}
		}
	
		last = cur_block;	// We need to save this in case we have to add a new block.
					// If we do, then without this we'd have to walk the list again from the beginning
					// to find the existing last block and update its next pointer to the block we
					// just added.
	} while (cur_block = cur_block->next);
	
	if (best){
		return best;
	}
	
	return new_kmalloc_block(last, size);
}

void kfree(void *p){
	kmalloc_block *b;
	
	// The following is arithmetic on a void pointer, which is not permitted per C standard.
	// However, per GCC documentation, as a nonstandard extension GCC permits pointer arithmetic
	// on void pointers with an assumption that the object size is 1.
	// At time of writing (2020-05-25), this is documented at https://gcc.gnu.org/onlinedocs/gcc/Pointer-Arith.html
	b = p - sizeof(kmalloc_block);
	
	b->used = false;
	
	// later on we'll add logic to combine with successive free blocks
	// but for now, for testing, keep it simple
	return;	
}

void *kmalloc(uint64_t size){
	kmalloc_block *cur_block;
	
	if (!kmalloc_block_list){
		cur_block = (kmalloc_block *)brk;
		kmalloc_block_list = cur_block;
		
		cur_block = new_kmalloc_block(0, size);
	} else {
		cur_block = find_avail_kmalloc_block_list(size);
	}
	
	if (!cur_block){
		return 0;
	} else {
		return cur_block->base;
	}
}

kmalloc_block *new_kmalloc_block(kmalloc_block *last, uint64_t size){
	kmalloc_block *new;
	
	if ((UINT64_T_MAX - (sizeof(kmalloc_block) + size - 1)) < (uint64_t)brk){		// out of address space
		return 0;
	}
	
	new = brk;
	
	// The following is arithmetic on a void pointer, which is not permitted per C standard.
	// However, per GCC documentation, as a nonstandard extension GCC permits pointer arithmetic
	// on void pointers with an assumption that the object size is 1.
	// At time of writing (2020-05-25), this is documented at https://gcc.gnu.org/onlinedocs/gcc/Pointer-Arith.html
	new->base = (void *)((uint64_t)new + sizeof(kmalloc_block));
	
	new->len = size;
	new->used = true;
	new->next = 0;
	
	if (last){
		last->next = new;
	}
	
	// more void pointer arithmetic
	brk += (sizeof(kmalloc_block) + size);
	
	return new;
}

#endif