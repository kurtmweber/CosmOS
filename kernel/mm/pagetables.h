/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PAGETABLES_H
#define _PAGETABLES_H

#include <types.h>

typedef uint64_t ptt_t;     // page translation table

#define DIRECT_MAP_OFFSET           0xFFFFA00000000000

#define MAX_ID_MAP                  0x100000

// Set this to somewhere above the last page tables that are created by the bootloader
#define EARLY_PAGE_TABLE_PHYS_BASE      0x30000

// This is the start of the Extended BIOS Data Area used by SMM.  Once we reach this point with our
// early page tables, we need to start using heap space
#define EARLY_PAGE_TABLE_NEVER_EXCEED   0x80000

#define PTT_EXTRACT_BASE(x) (x & 0x000FFFFFFFFFF000)
#define PTT_SET_BASE(ptt, base) (ptt |= base)
#define PTT_ADJUST_BASE(x) (((uint64_t)x < MAX_ID_MAP) ? (void *)x : (void *)((uint64_t)x + DIRECT_MAP_OFFSET))
#define CONV_PHYS_ADDR(x) PTT_ADJUST_BASE(x)
#define CONV_DMAP_ADDR(x) ((void *)((uint64_t)x - DIRECT_MAP_OFFSET))

// Flags to set (or not) in PTT entries
#define PTT_FLAG_PRESENT    1       // present in physical memory
#define PTT_FLAG_RW         2       // read-write (0 = read-only)
#define PTT_FLAG_USER       4       // whether to allow user access to page (0 = supervisor only)
#define PTT_FLAG_PWT        8       // writethrough caching (0 = writeback caching)
#define PTT_FLAG_PWD        16      // 0 = cacheable, 1 = disable caching
#define PTT_FLAG_ACCESSED   32      // has page been accessed?
#define PTT_FLAG_DIRTY      64      // has it been written to?
#define PTT_FLAG_PS         128     // page size - see AMD64 docs for information, for now we'll
                                    // use 4-kb pages so it should always be 0
#define PTT_FLAG_GLOBAL     256     // 1 for global page

typedef struct page_directory_t{
    uint64_t ref_count;
    uint64_t proc_count;
    uint64_t type;
    uint64_t flags;
} page_directory_t;

// pagetables.c
pttentry ptt_entry_create(void *base_address, bool present, bool rw, bool user);

// directmap.c
void setup_direct_map(int_15_map *phys_map, uint8_t num_blocks);

#endif