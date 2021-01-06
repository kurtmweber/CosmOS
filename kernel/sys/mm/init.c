/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/console/console.h>
#include <dev/i386/isadma/isadma.h>
#include <dev/virtio/virtqueue.h>

#include <sys/mm/mm.h>
#include <sys/mm/pagetables.h>

void mmu_init(){
	int_15_map *map;
	uint8_t num_blocks, lrg_block;
	uint8_t i;
	mem_block *b;
	page_directory_t *page_directory_start;
	
	brk = &_end;

	/*
	* ISA DMA buffers need to be in lower 64MB of RAM and page aligned
	*/
	isadma_buf = find_aligned_after(brk, ISA_DMA_ALIGNMENT);
	kprintf("   Reserved ISA DMA memory of size %#hX at %#hX\n", ISA_DMA_BUFSIZ, isadma_buf);
	brk = isadma_buf + ISA_DMA_BUFSIZ;

	/*
	* virtq buffers can be anywhere in RAM but do need to be page aligned
	*/
	virtqueue_buf = find_aligned_after(brk, VIRTQUEUE_ALIGNMENT);
	kprintf("   Reserved Virtqueue memory of size %#hX at %#hX\n",VIRTQUEUE_BUFSIZ, virtqueue_buf);
	brk = isadma_buf + VIRTQUEUE_BUFSIZ;
	
	kmalloc_init();
	
	map = read_int_15_map(&num_blocks, &lrg_block);

	page_directory_start = (page_directory_t *)setup_direct_map(map, num_blocks);
	
	setup_page_directory(page_directory_start, map, num_blocks);
	
	init_usable_phys_blocks(map[lrg_block]);
	
	enum_usable_phys_blocks(map, num_blocks);
	
	sort_usable_phys_blocks();

	reset_brk_after_malloc();

	// split off the first three blocks of physical memory we've allocated:
	// 0x0000000000000000 - 0x00000000000FFFFF: ID-mapped first megabyte
	// 0x0000000000100000 - 0x00000000008FFFFF: Kernel stack
	// 0x0000000000900000 - 0x0000000000AFFFFF: Kernel + kernel heap
	/*b = find_containing_block(0x0000000000000000, usable_phys_blocks);
	kprintf("Base: %llX\tLength: %llX\n", b->base, b->len);*/

	
	
	return;
}
