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
	uint8_t num_blocks, lrg_block;
	
	brk = &_end;
	kmalloc_block_list = 0;
	
	map = read_int_15_map(&num_blocks, &lrg_block);
	
	init_free_phys_blocks(map[lrg_block]);
}

#endif