/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PAGETABLES_C
#define _PAGETABLES_C

#include <types.h>
#include <asm/asm.h>
#include <mm/mm.h>
#include <panic/panic.h>
#include <string/string.h>


#ifdef COMPILE_PLATFORM_LINUX 
pttentry *extract_cr3_base_address(pttentry cr3) __attribute__((alias("extract_pttentry_base_address")));
#else
pttentry *extract_cr3_base_address(pttentry entry){
	return (pttentry *)(entry & PTTENTRY_BASE_MASK);
}
#endif

pttentry *extract_pttentry_base_address(pttentry entry){
	return (pttentry *)(entry & PTTENTRY_BASE_MASK);
}

bool is_page_aligned(void *address){
	
	// if modulus = 0, then it's page-aligned
	if (!((uint64_t)address % PAGE_SIZE)){
		return true;
	} else {
		return false;
	}
}

bool is_page_allocated(void *address){
	pttentry cr3;
	pttentry *pml4_base, *pdp_base, *pd_base, *pt_base;
	uint16_t pml4_index, pdp_index, pd_index, pt_index;
	pttentry pml4_entry, pdp_entry, pd_entry, pt_entry;
	
	cr3 = asm_cr3_read();
	
	pml4_base = extract_cr3_base_address(cr3);
	pml4_index = vaddr_ptt_index(address, PML4);	
	pml4_entry = pml4_base[pml4_index];
	if (!pml4_entry){
		return false;
	}
	
	pdp_base = extract_pttentry_base_address(pml4_entry);
	pdp_index = vaddr_ptt_index(address, PDP);
	pdp_entry = pdp_base[pdp_index];
	if (!pdp_entry){
		return false;
	}
	
	pd_base = extract_pttentry_base_address(pdp_entry);
	pd_index = vaddr_ptt_index(address, PD);
	pd_entry = pd_base[pd_index];
	
	if (!pd_entry){
		return false;
	}
	
	pt_base = extract_pttentry_base_address(pd_entry);
	pt_index = vaddr_ptt_index(address, PT);
	pt_entry = pt_base[pt_index];
	if (!pt_entry){
		return false;
	}
	
	return true;
}

uint16_t vaddr_ptt_index(void *address, ptt_levels level){
	uint64_t mask;
	uint8_t shift;
	switch (level){
		case PML4:
			mask = PML4_INDEX_MASK;
			shift = PML4_INDEX_SHIFT;
			break;
		case PDP:
			mask = PDP_INDEX_MASK;
			shift = PDP_INDEX_SHIFT;
			break;
		case PD:
			mask = PD_INDEX_MASK;
			shift = PD_INDEX_SHIFT;
			break;
		case PT:
			mask = PT_INDEX_MASK;
			shift = PT_INDEX_SHIFT;
			break;
		default:
			panic("Invalid PTT level provided!");
	}
	
	return ((uint64_t)address & mask) >> shift;
}

#endif