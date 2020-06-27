/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _MAP_C
#define _MAP_C

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <mm/mm.h>
#include <string/string.h>

const uint16_t map_base_addr = 0x500;
const uint8_t map_increment = 24;

int_15_map *read_int_15_map(uint8_t *num_blocks, uint8_t *lrg_block){
	int_15_map *map;
	uint8_t i = 0;
	
	map = (int_15_map *)((uint64_t)map_base_addr);
	
	*lrg_block = 0; // don't assume it's initialized by caller
	
	// boot loader places a block of all zeroes after the last memory block
	// so as long as at least one field is nonzero, it's a legit block
	// once we reach all zeroes, we know we're at the end
	while (map[i].base || map[i].len || map[i].type || map[i].acpi){
		if (map[i].type == USABLE){
			if (map[i].len > map[*lrg_block].len){
				*lrg_block = i;
			}
		}
		i++;
	}
	
	*num_blocks = i;
	
	return map;
}

#endif