/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <system/console/console.h>
#include <system/system_platform/i386/mm/mm.h>
#include <debug/assert.h>

void enum_usable_phys_blocks(int_15_map *map, uint8_t num_blocks){
	ASSERT_NOT_NULL(map, "map must not be null");
	mem_block *b;
	uint8_t i;
	
	b = usable_phys_blocks;
	
	for (i = 0; i < num_blocks; i++){
		if (map[i].type == USABLE){
			if (map[i].base != usable_phys_blocks->base){	// so we don't double up on the block we've already started the list with
				b->next = kmalloc(sizeof(mem_block));
				b->next->prev = b;
				b = b->next;
				b->next = 0;
				b->base = map[i].base;
				b->len = map[i].len - (map[i].len % PAGE_SIZE);
				b->used = false;
				b->owner = 0;
			}
		}
	}
	
	return;
}

mem_block *find_containing_block(void *addr, mem_block *list){
	ASSERT_NOT_NULL(addr, "addr must not be null");

	// returns NULL if the requested address is not found within a block in the list
	mem_block *tmp;

	tmp = list;

	do{
		if ((addr >= tmp->base) && (addr <= (tmp->base + tmp->len - 1))){
			return tmp;
		}

	} while (tmp = tmp->next);

	return NULL;
}

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

void sort_usable_phys_blocks(){
	mem_block *cur, *tmp;
	bool swapped = false;
	
	// just us a bubble sort, no point making it more complex than it has to be
	// there shouldn't ever be more than a handful of blocks, and this only runs once
	
	do {
		cur = usable_phys_blocks;
		swapped = false;
		while (cur){
			if (cur->next->base < cur->base){
				tmp = cur->next;
				cur->next = tmp->next;
				tmp->prev = cur->prev;
				tmp->next = cur;
				cur->prev = tmp;
			
				if (!tmp->prev){	// this means we've switched out the first element, so we need to update usable_phys_blocks
					usable_phys_blocks = tmp;
				}
			
				cur = tmp;
				
				swapped = true;
			} else {
				cur = cur->next;
			}
		}
	} while (swapped == false);
	
	return;
}
