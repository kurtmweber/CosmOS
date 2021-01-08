/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <sys/i386/mm/pagetables.h>
#include <sys/panic/panic.h>

page_directory_t *page_directory;

uint16_t find_page_bios_block(uint64_t page, int_15_map *phys_map, uint8_t num_blocks);
int_15_map_region_type get_page_bios_type(uint64_t page, int_15_map *phys_map, uint8_t num_blocks);
void init_page_directory(int_15_map *phys_map, uint8_t num_blocks);

uint16_t find_page_bios_block(uint64_t page, int_15_map *phys_map, uint8_t num_blocks){
    uint16_t i;

    /*
     * !!!!!!!!BE CAREFUL WITH THE RETURN VALUE FROM THIS FUNCTION!!!!!!!!
     * 
     * It will actually return the index of the proper block + 1.  This is so
     * that zero can be used to signify a hole (since holes do not have entries
     * in the block list). This is also why the return value is a uint16 rather
     * than a uint8, just to make sure we don't run into overflow problems if
     * there is a block at index 255.
     *
     * What this means is that you'll probably want to check if the return value
     * is zero; if it is treat it as a memory hole, otherwise subtract one and
     * use that as an index into the block list.
     */

    for (i = 0; i < num_blocks; i++){
        /*
         * We subtract 1 from base + len because len is a 1-based quantity and base is
         * 0-based, so base=0 and len=5 means that the end of the block is 4.
         */
        if (((uint64_t)phys_map[i].base <= (uint64_t)(page * PAGE_SIZE)) && ((uint64_t)(phys_map[i].base + phys_map[i].len - 1) >= (uint64_t)(page * PAGE_SIZE))){
            return i + 1;
        }
    }

    return 0;   // page is in a hole
}

int_15_map_region_type get_page_bios_type(uint64_t page, int_15_map *phys_map, uint8_t num_blocks){
    uint16_t i;

    i = find_page_bios_block(page, phys_map, num_blocks);

    if (!i){
        return HOLE;
    } else {
        return phys_map[i - 1].type;
    }
}

void init_page_directory(int_15_map *phys_map, uint8_t num_blocks){
    /*
     * This function creates blank page directory entries and fills them in with
     * the information we can glean from the BIOS memory map created at
     * boot--specifically the page types that the hardware tells us
     * (SYSTEM_RESERVED is determined later, since the hardware/BIOS doesn't
     * know about that)
     */
    uint64_t i;
    int_15_map_region_type bios_type;
    void *last_phys_addr;
    uint64_t num_phys_pages;

    last_phys_addr = find_last_phys_addr(phys_map, num_blocks);

    // How many pages does the physical address space encompass?
    num_phys_pages = ((uint64_t)last_phys_addr / PAGE_SIZE);
    if ((uint64_t)last_phys_addr % PAGE_SIZE){
        num_phys_pages++;
    }

    for (i = 0; i < num_phys_pages; i++){
        bios_type = get_page_bios_type(i, phys_map, num_blocks);

        page_directory[i].ref_count = 0;
        page_directory[i].backing_addr = 0;
        page_directory[i].flags = 0;

        switch(bios_type){
            case USABLE:
                page_directory[i].type = PDT_PHYS_AVAIL;
                break;
            // fall-through is intended behavior here
            case RESERVED:
            case ACPI_RECLAIM:
            case ACPI_NVS:
                page_directory[i].type = PDT_HARDWARE_RESERVED;
                break;
            case BAD:
                page_directory[i].type = PDT_BAD;
                break;
            case HOLE:
                page_directory[i].type = PDT_HOLE;
                break;
            default:
                panic("Invalid BIOS block type!");
        }
    }

    return;
}

void setup_page_directory(void *start, int_15_map *phys_map, uint8_t num_blocks){
    int_15_map best_block;
    void *dmap_start;
    uint64_t dmap_start_page;
    uint64_t i;
    void *last_phys_addr;
    uint64_t size;

    kprintf("Setting up physical page directory...\n");

    page_directory = start;
    
    init_page_directory(phys_map, num_blocks);

    /*
     * And now we update the system-reserved pages, starting with the ID-mapped
     * first megabyte.
     */

    for (i = 0; i < (1048576 / PAGE_SIZE); i++){
        /*
         * If a page is hardware-reserved, bad, or a hole, we don't mark it as
         * system-reserved.
         */
        if (page_directory[i].type == PDT_PHYS_AVAIL){
            page_directory[i].type = PDT_SYSTEM_RESERVED;
        }

        // But we increment its refcount regardless
        page_directory[i].ref_count++;
    }

    // Now the kernel text, heap, and stack space
    for (i = (1048576 / PAGE_SIZE); i < (BOOT_MAPPED_PHYS / PAGE_SIZE); i++){
        if (page_directory[i].type == PDT_PHYS_AVAIL){
            page_directory[i].type = PDT_SYSTEM_RESERVED;
        }

        page_directory[i].ref_count++;
    }

    // And, finally, the high direct map + page directory area
    last_phys_addr = find_last_phys_addr(phys_map, num_blocks);
    
    /*
     * The procedure here is to find the base of the block we put our upper
     * direct map at.  The page directory start immediately follows the direct
     * map, so by substracting page dir start from the base of that block, we
     * know the size of the upper direct map.  To that, we add the size of the
     * page directory.  At this point, we have the size in bytes of the upper
     * direct map + page directory.
     */

    /* 
     * +1 because we need a 1-based size, not a 0-based address which is what
     * last_phys_addr is
     */
    best_block = find_suitable_block(phys_map, num_blocks, (void *)BOOT_MAPPED_PHYS, (uint64_t)last_phys_addr + 1);
    dmap_start = best_block.base;
    size = (uint64_t)(CONV_DMAP_ADDR(start) - dmap_start);

    size += size_pd((uint64_t)last_phys_addr + 1);     // +1 for same reason as above

    /*
     *Now size = size in bytes of upper direct map + page directory, so we need
     * to know how many pages that encompasses.
     */
    if (size % PAGE_SIZE){
        size = (size / PAGE_SIZE) + 1;
    } else {
        size = (size / PAGE_SIZE);
    }

    dmap_start_page = (uint64_t)dmap_start / PAGE_SIZE;
    for (i = 0; i < size; i++){
        // Add to i the offset equal to the page of the start of the direct map.
        if (page_directory[i + dmap_start_page].type == PDT_PHYS_AVAIL){
            page_directory[i + dmap_start_page].type = PDT_SYSTEM_RESERVED;
        }
        page_directory[i + dmap_start_page].ref_count++;
    }

    return;
}