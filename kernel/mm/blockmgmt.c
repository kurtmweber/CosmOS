/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _BLOCKMGMT_C
#define _BLOCKMGMT_C

#include <types.h>
#include <mm/mm.h>

void init_usable_phys_blocks(int_15_map block){
	if (!is_page_aligned(block.base)){
		// clear the lower 12 bits of block.base so we get the first page boundary less than the block base
		// then add 4096 to get the first page boundary within the block
		init_phys_block.base = (void *)((((uint64_t)block.base >> 12) << 12) + 4096);
	} else {
		init_phys_block.base = block.base;
	}
	
	init_phys_block.len = block.len - (block.len % PAGE_SIZE);
	
	init_phys_block.used = false;
	init_phys_block.owner = 0;	// ignored for free blocks, but we'll set it to a defined value.
	
	init_phys_block.prev = 0;
	init_phys_block.next = 0;
	
	usable_phys_blocks = &init_phys_block;
	
	return;
}

#endif