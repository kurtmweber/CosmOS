/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <mm/pagetables.h>
#include <panic/panic.h>

int_15_map find_suitable_block(int_15_map *phys_map, uint8_t num_blocks, void *min, uint64_t space){
    // Find a block of uninterrupted physical address space, that starts abvoe min, of suitable size
    // for creating a direct map and page directory for a physical address space of size space
    uint8_t i;
    uint8_t best = 0;
    uint64_t needed_size;
    uint64_t pte, pde, pdpe, pml4;

    /*
     * First, we need enough space for the page table directory--this is the easiest to calculate
     * We can simplify this further if there is a guarantee that the physical address space
     * will encompass a whole number of pages, but I'm not sure there is
     */

    // How many pages are there in the physical address space?
    needed_size = (space / PAGE_SIZE);
    if (space % PAGE_SIZE){
        needed_size++;
    }
    // And then multiply by size of a page directory entry
    needed_size *= sizeof(page_directory_t);

    /* 
     * To that, we add the space needed for page tables for the entire physical space.
     * This is a bit more complicated because multi-level paging means we can't just
     * figure out how many pages we need and multiply by the size of a page-table entry.
     * Each level of the hierarchy contains 512 pointers to the next-lower level, so at each
     * level above the bottom, we divide by 512 one more time and round up since we always
     * have at least one table at each level.
     * How many levels depends on page size, and furthermore each table, regardless of how many
     * entries it has, takes up a full 4KB.
     */

    if (PAGE_SIZE == 4096){
        pte = (space / PAGE_SIZE) + ((space % PAGE_SIZE) ? 1 : 0);
        // since page table entries are 8 bytes long, and a table is 4k, that means 512 entries
        // in each page table, so we round up the # of PTEs we need to reserve
        // space for to the nearest 512
        // when we fill them out, we don't have to fill them all
        pte += ((pte % 512) ? (512 - (pte % 512)) : 0);

        // and then 512 PTEs in a pde, and so on up the hierarchy...
        pde = (pte / 512) + ((pte % 512) ? 1 : 0);
        pde += ((pde % 512) ? (512 - (pde % 512)) : 0);

        pdpe = (pde / 512) + ((pde % 512) ? 1 : 0);
        pdpe += ((pdpe % 512) ? (512 - (pdpe % 512)) : 0);
        
        // always 1 PML4 table, which is what cr3 points to
        pml4 = 1;
    } else {
        panic("Not prepared for current page size!");
    }

    // and then we adjust needed_size, keeping in mind that a page table entry is 8 bytes long
    needed_size += ((pte + pde + pdpe + pml4) * 8);

    for (i = 0; i < num_blocks; i++){
        /*
         * Criteria: the block must be usable, and must be of sufficient size for a direct map AND
         * page directory, AND be located past the end of what is currently mapped (so we don't
         * overwrite the running kernel or its data).
         * And then, of all blocks that meet this criteria, we want the smallest
         */
        if (((int_15_map_region_type)phys_map[i].type == USABLE) && (phys_map[i].len >= needed_size) && (phys_map[i].base >= (void *)BOOT_MAPPED_PHYS)){
            if (phys_map[best].len < needed_size){  // because best is initialized to 0...see note below
                best = i;
            }
            if (phys_map[i].len < phys_map[best].len){
                best = i;
            }
        }
    }

    /*
     * Sanity check because if the first block in the list is too small but is
     * also the largest block returned (extremely unlikely, but just to be safe)
     * then because best is initially set to 0, it will get returned as best
     * from the above loop, which is obviously not what we want.  It also means
     * that no blocks of sufficient size were found, which is a good reason to refuse
     * to continue
     */
    if (phys_map[best].len < needed_size){
        panic("No physical blocks of sufficient size found for direct map and page database!");
    }

    return phys_map[best];
}

void *find_last_phys_addr(int_15_map *phys_map, uint8_t num_blocks){
    uint8_t i;
    void *last_addr = 0;

    for (i = 0; i < num_blocks; i++){
        // subtract one because the base address is part of the len
        // so for example if the base is 0, and the len is 1, then the last address is 0
        // if base is 5 and len is 8, then the last address is 12, etc.
        if ((phys_map[i].base + phys_map[i].len - 1) > last_addr){
            last_addr = (phys_map[i].base + phys_map[i].len - 1);
        }
    }

    return last_addr;
}

void setup_direct_map(int_15_map *phys_map, uint8_t num_blocks){
    ptt_t cr3;
    uint16_t idx;
    int_15_map best_block;
    void *last_phys_addr = 0;
    pttentry *pml4;
    void *dmap_start = 0;

    last_phys_addr = find_last_phys_addr(phys_map, num_blocks);

    // last_phys_addr + 1 because the last address uses zero-bsed #ing, and we need the size of the space
    best_block = find_suitable_block(phys_map, num_blocks, (void *)BOOT_MAPPED_PHYS, (uint64_t)last_phys_addr + 1);

    dmap_start = best_block.base;
    kprintf("Best block base: 0x%llX\tLen: 0x%llX\n", best_block.base, best_block.len);

    /*
     * At this point, all ptts are in the ID-mapped first megabyte, so we can
     * use the virtual addresses directly
     */

    cr3 = asm_cr3_read();
    /*
     * Even though we're direct-mapping all the physical space, we'll start
     * with the block we found above so we can start putting stuff in it
     * ASAP, before we run out of room in the ID-mapped first megabyte
     */
    idx = vaddr_ptt_index(((void *)DIRECT_MAP_OFFSET) + best_block.len, PML4);
    kprintf("PML4 index: %u\n", idx);
    pml4 = (pttentry *)PTT_EXTRACT_BASE(cr3);
    


    /*
     * Bootloader clears PML4 area for us, so we can count on this being a reliable test as to whether
     * there is already an entry there or not.  If there is, we follow it; if not, we write a new one.
     */
    if (!pml4[idx]){
        //pml4[idx] = 
    }

    return;
}