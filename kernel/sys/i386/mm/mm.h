/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _MM_H
#define _MM_H

#include <sys/i386/mm/pagetables.h>
#include <sys/kmalloc/kmalloc.h>
#include <types.h>

// How much physical address space the bootloader has mapped
#define BOOT_MAPPED_PHYS 0x1100000

#define PTTENTRY_BASE_MASK 0x000FFFFFFFFFF000
#define INIT_UNMAPPED_PHYS_BASE 0xB00000

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

// PFE error flags
#define PFE_ERROR_PRESENT 1
#define PFE_ERROR_WRITE 2
#define PFE_ERROR_USER 4
#define PFE_ERROR_WRITE_R 8
#define PFE_ERROR_FETCH_INS 16

// defined in cosmos.ld
extern uint64_t _end;

// straight typedefs
typedef uint64_t pttentry;

// variables to fill in pagedirectory.c to inform where the IO buffers are
extern uint64_t io_buf;
extern uint64_t io_buf_bytes;

// enums

// forward declarations needed to avoid compilation errors
enum page_directory_types;
typedef enum page_directory_types page_directory_types;

typedef enum int_15_map_region_type {
    USABLE = 1,
    RESERVED = 2,
    ACPI_RECLAIM = 3,
    ACPI_NVS = 4,
    BAD = 5,
    HOLE  // Not an actual type returned by the int 15 map, but we use it in page-directory setup
} int_15_map_region_type;

typedef enum ptt_levels { PML4 = 0, PDP, PD, PT } ptt_levels;

// structs

typedef struct int_15_map {
    void* base;
    uint64_t len;
    uint32_t
        type;  // read this as int_15_map_region_type, but we store it as uint32_t in the struct to get the size right
    uint32_t acpi;
} __attribute__((packed)) int_15_map;

// blockmgmt.c
mem_block* find_containing_block(void* addr, mem_block* list);
int_15_map* read_int_15_map(uint8_t* num_blocks, uint8_t* lrg_block);

// init.c
extern uint64_t future_pt_expansion[3];
void mmu_init();

// mm.c
extern void* brk;
extern mem_block init_phys_block;
extern mem_block* usable_phys_blocks;
extern pttentry system_cr3;
void* find_aligned_after(void* address, uint64_t alignment);
void* find_last_phys_addr(int_15_map* phys_map, uint8_t num_blocks);

// pagefault.c
void page_fault_handler(uint64_t error, void* cr2, pttentry cr3);

// pagetables.c
bool is_page_aligned(void* address);
bool is_page_allocated(void* address);
pttentry* extract_cr3_base_address(pttentry entry);
pttentry* extract_pttentry_base_address(pttentry entry);
uint16_t vaddr_ptt_index(void* address, ptt_levels level);
void* vaddr_to_physical(void* address, pttentry cr3);

// slab.c
uint64_t slab_allocate(uint64_t pages, page_directory_types purpose);
void slab_free(uint64_t start, uint64_t len);

#endif