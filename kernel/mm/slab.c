/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <mm/mm.h>
#include <mm/pagetables.h>

uint64_t slab_allocate(uint64_t pages, page_directory_types purpose){
    /*
     * Allocates a CONTIGUOUS chunk of physical pages, and returns the
     * page-directory index of the first one.  If a sufficiently-sized chunk
     * cannot be found, return 0. Zero is a valid page-directory index, of
     * course, but it refers to the first page which is system-reserved and will
     * never be made available, so we can use it for these purposes.
     */

    //while (!__atomic_compare_exchange_n())
}