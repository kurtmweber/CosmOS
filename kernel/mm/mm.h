/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _MM_H
#define _MM_H

#include <types.h>

#define PTTENTRY_BASE_MASK 0x000FFFFFFFFFF000
#define INIT_UNMAPPED_PHYS_BASE	0xB00000

// AND these masks w/virtual addresses and then shift them right to get the index into the respective level of the page translation tables
#define PML4_INDEX_MASK 0x0000FF8000000000
#define PML4_INDEX_SHIFT 39
#define PDP_INDEX_MASK 0x0000007FC0000000
#define PDP_INDEX_SHIFT 30
#define PD_INDEX_MASK 0x000000003FE00000
#define PD_INDEX_SHIFT 21
#define PT_INDEX_MASK 0x00000000001FF000
#define PT_INDEX_SHIFT 12

#define KMALLOC_ALIGN_BYTES	8

#define PAGE_SIZE 4096

extern uint64_t _end;

// straight typedefs

typedef uint64_t pttentry;

// enums

typedef enum int_15_map_region_type{
	USABLE = 1,
	RESERVED = 2,
	ACPI_RECLAIM = 3,
	ACPI_NVS = 4,
	BAD = 5
} int_15_map_region_type;

typedef enum ptt_levels{
	PML4,
	PDP,
	PD,
	PT
} ptt_levels;

// structs

typedef struct int_15_map{
	void *base;
	uint64_t len;
	uint32_t type;		// read this as int_15_map_region_type, but we store it as uint32_t in the struct to get the size right
	uint32_t acpi;
}__attribute__((packed)) int_15_map;

typedef struct mem_block{
	struct mem_block *prev;
	void *base;
	uint64_t len;
	bool used;
	uint64_t owner;		// ignored for free blocks
	struct mem_block *next;
}__attribute__((aligned(8))) mem_block;

typedef mem_block kmalloc_block;

#ifndef _BLOCKMGMT_C
void enum_usable_phys_blocks(int_15_map *map, uint8_t num_blocks);
void init_usable_phys_blocks(int_15_map base);
void sort_usable_phys_blocks();

extern mem_block init_phys_block;
extern mem_block *usable_phys_blocks;
#else
mem_block init_phys_block;
mem_block *usable_phys_blocks;
#endif

#ifndef _INIT_C
void mmu_init();
#endif

#ifndef _KMALLOC_C
void kfree(void *p);
void *kmalloc(uint64_t size);
void kmalloc_init();
void *krealloc(void *ptr, uint64_t size);

extern void *brk;
#else
kmalloc_block *find_avail_kmalloc_block_list(uint64_t size);
kmalloc_block *new_kmalloc_block(kmalloc_block *last, uint64_t size);

kmalloc_block *kmalloc_block_list;
kmalloc_block *kmalloc_block_list_end;
void *brk;
#endif

#ifndef _MAP_C
int_15_map *read_int_15_map(uint8_t *num_blocks, uint8_t *lrg_block);

extern const uint16_t page_size;
#else
const uint16_t page_size = 4096;
#endif

#ifndef _MM_PHYS_ALLOC_C
mem_block *phys_alloc_slab(uint64_t size, uint64_t align);
#else
mem_block *phys_split_block(mem_block *src, void *base, uint64_t size);
#endif

#ifndef _PAGETABLES_C
bool is_page_aligned(void *address);
bool is_page_allocated(void *address);
#else
pttentry *extract_cr3_base_address(pttentry entry);
pttentry *extract_pttentry_base_address(pttentry entry);
uint16_t vaddr_ptt_index(void *address, ptt_levels level);
#endif

#endif