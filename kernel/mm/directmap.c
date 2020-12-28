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

void setup_direct_map(int_15_map *phys_map){
    ptt_t cr3;
    uint16_t idx;
    pttentry*pml4;

    // for now, all ptts are in the ID-mapped first megabyte, so we can
    // use the virtual addresses directly

    cr3 = asm_cr3_read();

    idx = vaddr_ptt_index((void *)DIRECT_MAP_OFFSET, PML4);
    pml4 = PTT_EXTRACT_BASE(cr3);
    
    /// bootloader clears PML4 area for us, so we can count on this being a reliable test
    if (!pml4[idx]){
        kprintf("Hi\n");
    }

    return;
}