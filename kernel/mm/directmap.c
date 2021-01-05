/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <mm/pagetables.h>
#include <panic/panic.h>
#include <string/mem.h>

int_15_map find_suitable_block(int_15_map *phys_map, uint8_t num_blocks, void *min, uint64_t space);
uint64_t size_pd(uint64_t space);

void *adjust_cur_phys_loc(void *cur_phys_loc, void *safe_base){
    /*
     * If we're about to exceed the limit for the ID-mapped first megabyte page
     * tables, then we jump up to the direct map physical base.  The order of
     * the comparisons is important for efficiency--short-circuit evaluation
     * means that the second comparison is never checked once we pass the limit.
     */
    if ((cur_phys_loc <= (void *)EARLY_PAGE_TABLE_NEVER_EXCEED) && (cur_phys_loc + PAGE_SIZE >= (void *)EARLY_PAGE_TABLE_NEVER_EXCEED)){
        return safe_base;
    } else {
        return cur_phys_loc + PAGE_SIZE;
    }
}

int_15_map find_suitable_block(int_15_map *phys_map, uint8_t num_blocks, void *min, uint64_t space){
    // Find a block of uninterrupted physical address space, that starts above min, of suitable size
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

    needed_size = size_pd(space);
    
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

void *setup_direct_map(int_15_map *phys_map, uint8_t num_blocks){
    ptt_t cr3;
    void *active_virt_loc;
    void *cur_phys_loc = (void *)EARLY_PAGE_TABLE_PHYS_BASE;
    uint64_t i;
    uint16_t idx;
    int_15_map best_block;
    void *last_phys_addr = 0;
    uint64_t num_phys_pages;
    pttentry *pml4, *pdp, *pd, *pt;
    void *dmap_start = 0;

    kprintf("Initializing direct map...\n");

    last_phys_addr = find_last_phys_addr(phys_map, num_blocks);

    // How many pages does the physical address space encompass?
    num_phys_pages = ((uint64_t)last_phys_addr / PAGE_SIZE);
    if ((uint64_t)last_phys_addr % PAGE_SIZE){
        num_phys_pages++;
    }

    /*
     * last_phys_addr + 1 because the last address uses zero-bsed #ing, and we
     * need the size of the space
     *
     * If for whatever reason the method for choosing the physical block for the
     * direct map changes, updates to setup_page_directory() in pagedirectory.c
     * may become necessary too, particularly if those changes are outside of
     * the find_suitable_block() function or change its signature (though those
     * will trigger compiler errors so that should be an automatic reminder, at
     * least).  This is because setup_page_directory() has to know what physical
     * pages are system-reserved to set the type in the page directory properly.
     */
    best_block = find_suitable_block(phys_map, num_blocks, (void *)BOOT_MAPPED_PHYS, (uint64_t)last_phys_addr + 1);

    dmap_start = best_block.base;

    /*
     * At this point, all ptts are in the ID-mapped first megabyte, so we can
     * use the virtual addresses directly
     */

    cr3 = asm_cr3_read();

    /*
     * Now we loop through.  We start at the virtual address that will
     * correspond to the first address of the physical block we're using, and
     * map pages until we reach the end of the address space.  Then we do
     * another loop to start at 0, and stop when we get up to where we started.
     * Even though we're direct-mapping all the physical space, we'll start with
     * the block we found above so we can start putting stuff in it ASAP, before
     * we run out of room in the ID-mapped first megabyte      
     */

    /*
     * Zero-based numbering works to our advantage here: because unsigned
     * integer division truncates, the value of best_block.base/PAGE_SIZE will
     * be the zero-based index of the page #. For example, 0/4096 = 0,
     * 4095/4096=0, 4096/4096=1, etc. Keep in mind, though, that num_phys_pages
     * is a 1-based count.
     */
     
    for (i = (uint64_t)best_block.base / PAGE_SIZE; i < num_phys_pages; i++){
        /*
         * This loop performs a lot of unncessary checks of the higher levels
         * of the page-table hierarchy.  We can optimize that later if it
         * becomes an issue.
         */

        active_virt_loc = (void *)((i * PAGE_SIZE) + DIRECT_MAP_OFFSET);

        pml4 = (pttentry *)PTT_EXTRACT_BASE(cr3);
        idx = vaddr_ptt_index(active_virt_loc, PML4);

        /*
         * Bootloader clears PML4 area for us, so we can count on this being a
         * reliable test as to whether there is already an entry there or not.  If
         * there is, we follow it; if not, we write a new one.
         */
        if (!pml4[idx]){
            // Clear the page table we're about to point to in PML4 and set up
            memset(CONV_PHYS_ADDR(cur_phys_loc), 0, PAGE_SIZE);
            pml4[idx] = ptt_entry_create(cur_phys_loc, true, true, false);
            cur_phys_loc = adjust_cur_phys_loc(cur_phys_loc, best_block.base);
        }

        pdp = (pttentry *)PTT_ADJUST_BASE(PTT_EXTRACT_BASE(pml4[idx]));
        idx = vaddr_ptt_index(active_virt_loc, PDP);

        /*
         * And, similarly, bootloader or kernel will clear newly-assigned page
         * table areas at all levels.
         */

        if (!pdp[idx]){
            memset(CONV_PHYS_ADDR(cur_phys_loc), 0, PAGE_SIZE);
            pdp[idx] = ptt_entry_create(cur_phys_loc, true, true, false);
            cur_phys_loc = adjust_cur_phys_loc(cur_phys_loc, best_block.base);
        }

        pd = (pttentry *)PTT_ADJUST_BASE(PTT_EXTRACT_BASE(pdp[idx]));
        idx = vaddr_ptt_index(active_virt_loc, PD);

        if (!pd[idx]){
            memset(CONV_PHYS_ADDR(cur_phys_loc), 0, PAGE_SIZE);
            pd[idx] = ptt_entry_create(cur_phys_loc, true, true, false);
            cur_phys_loc = adjust_cur_phys_loc(cur_phys_loc, best_block.base);
        }

        pt = (pttentry *)PTT_ADJUST_BASE(PTT_EXTRACT_BASE(pd[idx]));
        idx = vaddr_ptt_index(active_virt_loc, PT);

        if (!pt[idx]){
            pt[idx] = ptt_entry_create((void *)(i * PAGE_SIZE), true, true, false);
        }

        asm_cr3_reload();
    }

    // And now, starting from 0 up to where we started the last loop

    for (i = 0; i < (uint64_t)best_block.base / PAGE_SIZE; i++){
        /*
         * This loop performs a lot of unncessary checks of the higher levels
         * of the page-table hierarchy.  We can optimize that later if it
         * becomes an issue.
         */

        active_virt_loc = (void *)((i * PAGE_SIZE) + DIRECT_MAP_OFFSET);

        pml4 = (pttentry *)PTT_EXTRACT_BASE(cr3);
        idx = vaddr_ptt_index(active_virt_loc, PML4);

        /*
         * Bootloader clears PML4 area for us, so we can count on this being a
         * reliable test as to whether there is already an entry there or not.  If
         * there is, we follow it; if not, we write a new one.
         */
        if (!pml4[idx]){
            // Clear the page table we're about to point to in PML4 and set up
            memset(CONV_PHYS_ADDR(cur_phys_loc), 0, PAGE_SIZE);
            pml4[idx] = ptt_entry_create(cur_phys_loc, true, true, false);
            cur_phys_loc = adjust_cur_phys_loc(cur_phys_loc, best_block.base);
        }

        pdp = (pttentry *)PTT_ADJUST_BASE(PTT_EXTRACT_BASE(pml4[idx]));
        idx = vaddr_ptt_index(active_virt_loc, PDP);

        /*
         * And, similarly, bootloader or kernel will clear newly-assigned page
         * table areas at all levels.
         */

        if (!pdp[idx]){
            memset(CONV_PHYS_ADDR(cur_phys_loc), 0, PAGE_SIZE);
            pdp[idx] = ptt_entry_create(cur_phys_loc, true, true, false);
            cur_phys_loc = adjust_cur_phys_loc(cur_phys_loc, best_block.base);
        }

        pd = (pttentry *)PTT_ADJUST_BASE(PTT_EXTRACT_BASE(pdp[idx]));
        idx = vaddr_ptt_index(active_virt_loc, PD);

        if (!pd[idx]){
            memset(CONV_PHYS_ADDR(cur_phys_loc), 0, PAGE_SIZE);
            pd[idx] = ptt_entry_create(cur_phys_loc, true, true, false);
            cur_phys_loc = adjust_cur_phys_loc(cur_phys_loc, best_block.base);
        }

        pt = (pttentry *)PTT_ADJUST_BASE(PTT_EXTRACT_BASE(pd[idx]));
        idx = vaddr_ptt_index(active_virt_loc, PT);

        if (!pt[idx]){
            pt[idx] = ptt_entry_create((void *)(i * PAGE_SIZE), true, true, false);
        }

        asm_cr3_reload();
    }
    
    // Return the first address after the direct-map page tables
    return CONV_PHYS_ADDR(cur_phys_loc);
}

uint64_t size_pd(uint64_t space){
    uint64_t needed_size;

    // How many pages are there in the physical address space?
    needed_size = (space / PAGE_SIZE);
    if (space % PAGE_SIZE){
        needed_size++;
    }

    // And then multiply by size of a page directory entry
    needed_size *= sizeof(page_directory_t);

    return needed_size;
}