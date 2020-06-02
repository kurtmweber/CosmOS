/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _INIT_C
#define _INIT_C

#include <debug_error/debug_error.h>
#include <mm/mm.h>

void mmu_init(){
	int_15_map *map;
	uint8_t num_blocks, lrg_block, num_usable_blocks = 0;
	uint8_t i;
	
	brk = &_end;
	kmalloc_block_list = 0;
	
	map = read_int_15_map(&num_blocks, &lrg_block);
	
	init_usable_phys_blocks(map[lrg_block]);
	
	for (i = 0; i < num_blocks; i++){
		if (map[i].type == USABLE){
			num_usable_blocks++;
		}
	}
	
	
}

#endif