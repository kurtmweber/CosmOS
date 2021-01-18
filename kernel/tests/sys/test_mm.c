//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/i386/mm/pagetables.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <tests/sys/test_mm.h>

void test_mm() {
    mem_block *tmp;
    tmp = usable_phys_blocks;

    do {
        kprintf("Base: %llX, Length: %llX\n", (uint64_t)tmp->base, tmp->len);
    } while ((tmp = tmp->next));
}

void test_pagetables() {
}
