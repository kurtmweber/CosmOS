/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <console/console.h>
#include <mm/mm.h>
#include <mm/pagetables.h>
#include <panic/panic.h>

page_directory_t *page_directory;

uint16_t find_page_bios_block(uint64_t page, int_15_map *phys_map, uint8_t num_blocks);
int_15_map_region_type get_page_bios_type(uint64_t page, int_15_map *phys_map, uint8_t num_blocks);
void init_page_directory(int_15_map *phys_map, uint8_t num_blocks);

uint16_t find_page_bios_block(uint64_t page, int_15_map *phys_map, uint8_t num_blocks){
    uint8_t i;

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
    uint8_t i;

    i = find_page_bios_block(page, phys_map, num_blocks);

    if (page <= 511){
        kprintf("Page %llu is in block %hu\n", page, i);
    }

    return 0;
}

void init_page_directory(int_15_map *phys_map, uint8_t num_blocks){
    /*
     * This function creates blank page directory entries and fills them in with
     * the information we can glean from the BIOS memory map created at boot.
     */
    uint64_t i;
    int_15_map_region_type bios_type;
    void *last_phys_addr;
    uint64_t num_phys_pages;

    last_phys_addr = find_last_phys_addr(phys_map, num_blocks);

    /*
    for (i = 0; i < num_blocks; i++){
        kprintf("Block %llu, base 0x%llX, len %llu, type %lu\n", i, (uint64_t)phys_map[i].base, phys_map[i].len, phys_map[i].type);
    }*/

    // How many pages does the physical address space encompass?
    num_phys_pages = ((uint64_t)last_phys_addr / PAGE_SIZE);
    if ((uint64_t)last_phys_addr % PAGE_SIZE){
        num_phys_pages++;
    }

    for (i = 0; i < num_phys_pages; i++){
        bios_type = get_page_bios_type(i, phys_map, num_blocks);
    }
}

void setup_page_directory(void *start, int_15_map *phys_map, uint8_t num_blocks){
    uint64_t i;

    kprintf("Setting up physical page directory...\n");

    page_directory = start;

    init_page_directory(phys_map, num_blocks);

    return;
}