//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <mm/mm.h>

mem_block init_phys_block;
mem_block *usable_phys_blocks;

void *find_aligned_after(void *address, uint64_t alignment){
    // given an address, returns the next *alignment*-aligned address after it
    void *retval;
    uint64_t rem;

    rem = (uint64_t)address % alignment;
    retval = (void *)((alignment - rem) + (uint64_t)address);

    return retval;
}

void *brk;

