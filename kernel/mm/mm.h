/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _MM_H
#define _MM_H

#include <types.h>

#define PTTENTRY_BASE_MASK 0x000FFFFFFFFFF000

// AND these masks w/virtual addresses and then shift them right to get the index into the respective level of the page translation tables
#define PML4_INDEX_MASK 0x0000FF8000000000
#define PML4_INDEX_SHIFT 39
#define PDP_INDEX_MASK 0x0000007FC0000000
#define PDP_INDEX_SHIFT 30
#define PD_INDEX_MASK 0x000000003FE00000
#define PD_INDEX_SHIFT 21
#define PT_INDEX_MASK 0x00000000001FF000
#define PT_INDEX_SHIFT 12

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
	uint32_t type;		// read this as int15MapRegionType, but we store it as uint32_t in the struct to get the size right
	uint32_t acpi;
}__attribute__((packed)) int_15_map;

typedef struct kmalloc_block{
	void *base;
	uint64_t len;
	bool used;
	struct kmalloc_block *next;
} kmalloc_block;

typedef struct mem_block{
	struct mem_block *prev;
	void *base;
	uint64_t len;
	bool free;
	uint64_t owner;		// ignored for free blocks
	struct mem_block *next;
} mem_block;

#ifndef _BLOCKMGMT_C
void init_free_phys_blocks(int_15_map base);

extern mem_block init_phys_block;
extern mem_block *free_phys_blocks;
#else
mem_block init_phys_block;
mem_block *free_phys_blocks;
#endif

#ifndef _INIT_C
void mmu_init();
#endif

#ifndef _KMALLOC_C
void *kmalloc(uint64_t size);

extern kmalloc_block *kmalloc_block_list;
extern void *brk;
#else
kmalloc_block *find_avail_kmalloc_block_list(uint64_t size);
kmalloc_block *new_kmalloc_block(kmalloc_block *last, uint64_t size);

kmalloc_block *kmalloc_block_list;
void *brk;
#endif

#ifndef _MAP_C
int_15_map *read_int_15_map(uint8_t *num_blocks, uint8_t *lrg_block);

extern const uint16_t page_size;
#else
const uint16_t page_size = 4096;
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