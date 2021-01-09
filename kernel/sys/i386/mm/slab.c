/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/i386/mm/mm.h>
#include <sys/i386/mm/pagetables.h>
#include <sys/sync/sync.h>

uint64_t slab_allocate(uint64_t pages, page_directory_types purpose){
    /*
     * Allocates a CONTIGUOUS chunk of physical pages, and returns the
     * page-directory index of the first one.  If a sufficiently-sized chunk
     * cannot be found, return 0. Zero is a valid page-directory index, of
     * course, but it refers to the first page which is system-reserved and will
     * never be made available, so we can use it for these purposes.
     */

    uint64_t consecutive_pages = 0;
    uint64_t i;
    uint64_t start_page = 0;
    
    spinlock_acquire(&mem_lock);

    /*
     * Eventually, make this faster by implementing a linked list of blocks of
     * free pages
     */

    for (i = 0; i < page_directory_size; i++){
        if (page_directory[i].ref_count == 0){
            /*
             * If this is the first free block, or the last free block was of
             * insufficient size, then we mark this page as the start of the
             * current free block.  If it's the start of a free block OR a
             * continuation, we increment the # of consecutive free pages.
             */
            if (start_page == 0){
                start_page = i;
            }
            consecutive_pages++;

            // Return if we've got what we need
            if (consecutive_pages == pages){
                spinlock_release(&mem_lock);
                return start_page;
            }
        } else {
            /*
            * If we get here, then we've found an unfree page, so we reset the
            * counter and the start_page marker.
            */
           start_page = 0;
           consecutive_pages = 0;
        }
    }

    spinlock_release(&mem_lock);

    // If we get here, we never found a block of sufficient size, return 0
    return 0;
}