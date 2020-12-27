/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <console/console.h>
#include <mm/mm.h>

void mmu_init(){
	int_15_map *map;
	uint8_t num_blocks, lrg_block;
	uint8_t i;
	mem_block *b;
	
	brk = &_end;
	
	kmalloc_init();
	
	map = read_int_15_map(&num_blocks, &lrg_block);

	for (i = 0; i < num_blocks; i++){
		kprintf("Base: %llX\n Len: %llX\n Type: %hu\n", (uint64_t)map[i].base, (uint64_t)map[i].len, (uint8_t)map[i].type);

	}
	
	init_usable_phys_blocks(map[lrg_block]);
	
	enum_usable_phys_blocks(map, num_blocks);
	
	sort_usable_phys_blocks();

	reset_brk_after_malloc();

	kprintf("New brk: %llX\n", (uint64_t *)brk);

	// split off the first three blocks of physical memory we've allocated:
	// 0x0000000000000000 - 0x00000000000FFFFF: ID-mapped first megabyte
	// 0x0000000000100000 - 0x00000000008FFFFF: Kernel stack
	// 0x0000000000900000 - 0x0000000000AFFFFF: Kernel + kernel heap
	/*b = find_containing_block(0x0000000000000000, usable_phys_blocks);
	kprintf("Base: %llX\tLength: %llX\n", b->base, b->len);*/

	
	
	return;
}
