/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>

kmalloc_block *kmalloc_block_list;
kmalloc_block *kmalloc_block_list_end;

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
			
		last = cur_block;	// We need to save this in case we have to add a new block.
					// If we do, then without this we'd have to walk the list again from the beginning
					// to find the existing last block and update its next pointer to the block we
					// just added.
					
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

	} while ((cur_block = cur_block->next));
	
	if (best){
		return best;
	}
	
	return new_kmalloc_block(last, size);
}

void kfree(void *p){
	ASSERT_NOT_NULL(p, "p must not be null");
	kmalloc_block *b;
	
	// The following is arithmetic on a void pointer, which is not permitted per C standard.
	// However, per GCC documentation, as a nonstandard extension GCC permits pointer arithmetic
	// on void pointers with an assumption that the object size is 1.
	// At time of writing (2020-05-25), this is documented at https://gcc.gnu.org/onlinedocs/gcc/Pointer-Arith.html
	b = p - sizeof(kmalloc_block);
	
	b->used = false;
	
	if ((b->next) && (b->next->used == false)){
		b->len = b->len + sizeof(kmalloc_block) + b->next->len;
		b->next = b->next->next;
	}
	
	if ((b->prev) && (b->prev->used == false)){
		b->prev->len = b->prev->len + sizeof(kmalloc_block) + b->len;
		b->prev->next = b->next;
	}
	
	return;	
}

void *kmalloc(uint64_t size){
	kmalloc_block *cur_block;
	
	if (size % KMALLOC_ALIGN_BYTES){
		size += (KMALLOC_ALIGN_BYTES - (size % KMALLOC_ALIGN_BYTES));
	}
	
	if (!kmalloc_block_list){
		if ((uint64_t)brk % KMALLOC_ALIGN_BYTES){
			cur_block = (kmalloc_block *)(brk + (KMALLOC_ALIGN_BYTES - ((uint64_t)brk % KMALLOC_ALIGN_BYTES)));
		} else {
			cur_block = (kmalloc_block *)brk;
		}
		
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

void *kmalloc_align_block_end(kmalloc_block *block, uint64_t alignment){
	ASSERT_NOT_NULL(block, "block must not be null");

	// Expands block->len by 0 or more bytes, so that
	// e.g. if block->base is 14, and alignment is 8, then block->len
	// will be set to 2; this way the block ends at address 15

	// If the block is already sized so that it ends at an appropriate address,
	// then block->len remains unchanged

	// This is used primarily to expand the last block in a page
	// in preparation for setting aside succeeding pages for reserved uses
	// such as DMA areas, page tables, etc. that need to be withheld from
	// the heap space available to kmalloc(), but it conceivably might
	// have other uses as well.

	// returns the first address SUBSEQUENT to the end of the block, so e.g.
	// in the above example it returns 16

	void *end_addr;
	uint64_t diff;

	end_addr = block->base + block->len;
	diff = (uint64_t)end_addr % alignment;
	if (!diff){
		return end_addr;
	}

	// EXAMPLE:
	// alignment = 8, block->base = 2, block->len = 9
	// so end_addr = 11, therefore diff = 3
	// alignment - diff = 5, so we need to add 5 to block->len
	// block->len is now 14, and block->base + block->len = 16, which 
	// is the beginning of a new 8-byte-aligned block

	block->len += (alignment - diff);
	end_addr = block->base + block->len;
	return end_addr;
}

void kmalloc_init(){
	kmalloc_block_list = 0;
	kmalloc_block_list_end = 0;
	
	return;
}
/*
* last can be null here
*/
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
	new->prev = last;
	new->owner = 0;
	
	if (last){
		last->next = new;
	}
	
	// more void pointer arithmetic
	brk += (sizeof(kmalloc_block) + size);
	
	kmalloc_block_list_end = new;
	
	return new;
}

void *krealloc(void *ptr, uint64_t size){
	ASSERT_NOT_NULL(ptr, "ptr must not be null");

	kmalloc_block *b;
	void *new_block;
	BYTE *dest, *src;
	uint64_t i;
	
	b = ptr - sizeof(kmalloc_block);
	
	// to keep it simple, if the new size is less than the old size we just leave it alone
	// we might fix this later if memory pressure becomes an issues
	if (size <= b->len){
		return ptr;
	}
	
	// keep everything 8-byte aligned
	if (size % KMALLOC_ALIGN_BYTES){
		size += (KMALLOC_ALIGN_BYTES - (size % KMALLOC_ALIGN_BYTES));
	}
	
	// if it's the last block, then instead of creating a new block we just extend the existing one	
	if (!(b->next)){
		b->len = size;
		brk = ptr + size;
		return ptr;
	}
	
	// otherwise we grab a new block of the requested size, copy over the data, and free the old one
	new_block = kmalloc(size);
	src = (BYTE *)ptr;
	dest = (BYTE *)new_block;
	for (i = 0; i < b->len; i++){
		*dest = *src;
		src++;
		dest++;
	}
	
	kfree(ptr);
	return new_block;
}

void reset_brk_after_malloc(){
	brk = kmalloc_align_block_end(kmalloc_block_list_end, 4096);

	return;
}