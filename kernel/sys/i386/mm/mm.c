//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/i386/mm/mm.h>

mem_block init_phys_block;
mem_block *usable_phys_blocks;

pttentry system_cr3;

void *find_aligned_after(void *address, uint64_t alignment){
    // given an address, returns the next *alignment*-aligned address after it
    void *retval;
    uint64_t rem;

    rem = (uint64_t)address % alignment;
    retval = (void *)((alignment - rem) + (uint64_t)address);

    return retval;
}

void *find_last_phys_addr(int_15_map *phys_map, uint8_t num_blocks){
    uint8_t i;
    void *last_addr = 0;

    for (i = 0; i < num_blocks; i++){
        /*
         * Subtract one because the base address is part of the length. So for
         * example if the base is 0, and the len is 1, then the last address is
         * 0; if base is 5 and len is 8, then the last address is 12, etc.
         */
        if ((phys_map[i].base + phys_map[i].len - 1) > last_addr){
            last_addr = (phys_map[i].base + phys_map[i].len - 1);
        }
    }

    return last_addr;
}

void *brk;

