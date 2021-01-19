/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PAGETABLES_H
#define _PAGETABLES_H

#include <sys/i386/mm/mm.h>
#include <types.h>

typedef uint64_t ptt_t;  // page translation table

#define DIRECT_MAP_OFFSET 0xFFFFA00000000000

#define MAX_ID_MAP 0x100000

// Set this to somewhere above the last page tables that are created by the bootloader
#define EARLY_PAGE_TABLE_PHYS_BASE 0x30000

// This is the start of the Extended BIOS Data Area used by SMM.  Once we reach this point with our
// early page tables, we need to start using heap space
#define EARLY_PAGE_TABLE_NEVER_EXCEED 0x80000

#define PTT_EXTRACT_BASE(x) (x & 0x000FFFFFFFFFF000)
#define PTT_SET_BASE(ptt, base) (ptt |= base)

/*
 * Given a physical address, add the necessary offset to make a virtual address
 * within the direct-map area.
 */
#define PTT_ADJUST_BASE(x) (((uint64_t)x < MAX_ID_MAP) ? (void *)x : (void *)((uint64_t)x + DIRECT_MAP_OFFSET))
#define CONV_PHYS_ADDR(x) PTT_ADJUST_BASE(x)

// And vice-versa
#define CONV_DMAP_ADDR(x) ((void *)((uint64_t)x - DIRECT_MAP_OFFSET))

// Flags to set (or not) in PTT entries
#define PTT_FLAG_PRESENT 1    // present in physical memory
#define PTT_FLAG_RW 2         // read-write (0 = read-only)
#define PTT_FLAG_USER 4       // whether to allow user access to page (0 = supervisor only)
#define PTT_FLAG_PWT 8        // writethrough caching (0 = writeback caching)
#define PTT_FLAG_PWD 16       // 0 = cacheable, 1 = disable caching
#define PTT_FLAG_ACCESSED 32  // has page been accessed?
#define PTT_FLAG_DIRTY 64     // has it been written to?
#define PTT_FLAG_PS \
    128                      // page size - see AMD64 docs for information, for now we'll
                             // use 4-kb pages so it should always be 0
#define PTT_FLAG_GLOBAL 256  // 1 for global page

typedef enum page_directory_types {
    PDT_PHYS_AVAIL,                       // Physical memory available for allocation
    PDT_SYSTEM_RESERVED,                  // Reserved by operating system
    PDT_HARDWARE_RESERVED,                // Reserved by hardware or BIOS - NEVER ALLOCATE
    PDT_HOLE,                             // Hole in memory map - NEVER ALLOCATE
    PDT_BAD,                              // Flagged as bad by BIOS - NEVER ALLOCATE
    PDT_INUSE,                            // generic in-use flag
    PDT_SYSTEM_IO,                        // IO buffers
    PDT_MAX_PD_TYPE = 0xFFFFFFFFFFFFFFFF  // to force enum to 64 bits
} page_directory_types;

// forward type declarations to avoid compilation errors
struct int_15_map;
typedef struct int_15_map int_15_map;

enum ptt_levels;
typedef enum ptt_levels ptt_levels;

// use these as memos of what we're doing and when we need to convert
typedef void *phys_addr;
typedef void *virt_addr;

typedef uint64_t pttentry;

typedef struct page_directory_t {
    uint64_t ref_count;
    union {
        uint64_t backing_handle;
        void *backing_addr;
    };
    page_directory_types type;
    uint64_t flags;
} __attribute__((packed)) page_directory_t;

// directmap.c
void *setup_direct_map(int_15_map *phys_map, uint8_t num_blocks);
int_15_map find_suitable_block(int_15_map *phys_map, uint8_t num_blocks, void *min, uint64_t space);
uint64_t size_pd(uint64_t space);

// pagedirectory.c
extern page_directory_t *page_directory;
extern uint64_t page_directory_size;  // number of ENTRIES, not number of bytes
void setup_page_directory(void *start, int_15_map *phys_map, uint8_t num_blocks);

// pagetables.c
void add_pt_page(virt_addr *vaddr, uint64_t page, pttentry parent_entry, bool user);
void map_page_at(uint64_t page, void *vaddr, pttentry pml4_entry, bool user);
pttentry obtain_ptt_entry(virt_addr *vaddr, pttentry parent_entry, ptt_levels level, bool user);
pttentry ptt_entry_create(void *base_address, bool present, bool rw, bool user);
void reserve_next_ptt(ptt_levels level, uint64_t *expansion);

#endif