//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/i386/process/gdt.h>

// https://github.com/hach-que/Kernel

#define GDT_SIZE 3

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

// GDT
struct gdt_entry gdt[GDT_SIZE];

// pointer to GDT
struct gdt_ptr gdt_pointer;

/*
 * Setup a descriptor in the Global Descriptor Table
 */
void gdt_set_gate(uint8_t num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran) {
    ASSERT(num < GDT_SIZE);
    /*
     * Setup the descriptor base access
     */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    /*
     * Setup the descriptor limits
     */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = gran;

    /*
     * Finally, set up the granularity and access flags
     */
    //  gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_dump() {
    kprintf("Pointer Base: 0x%llX Limit: 0x%llX\n", gdt_pointer.base, gdt_pointer.limit);
    for (uint8_t i = 0; i < GDT_SIZE; i++) {
        kprintf("Entry 0x%llX\n", i);
        kprintf("   Base Low 0x%llX Middle 0x%llX High 0x%llX\n", gdt[i].base_low, gdt[i].base_middle, gdt[i].base_high);
        kprintf("   Limit Low 0x%llX\n", gdt[i].limit_low);
        kprintf("   Granularity 0x%llX\n", gdt[i].granularity);
        kprintf("   Access 0x%llX\n", gdt[i].access);
    }
}

void gdt_install() {
    /*
     * Setup the GDT pointer and limit
     */
    gdt_pointer.limit = (sizeof(struct gdt_entry) * GDT_SIZE) - 1;
    gdt_pointer.base = (uint64_t)&gdt;

    /*
     * Our NULL descriptor
     */
    gdt_set_gate(0, 0, 0xFFFFFFFF, 0, 1);

    /*
     * code segment
     */
    // 10011010 = 0x9A
    // 10101111 = 0xAF
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF);

    /*
     * data segment
     */
    // 10010010 = 0x92
    // 00000000 = 0
    gdt_set_gate(2, 0, 0, 0x92, 0x00);

    /*
     *Install the user mode segments into the GDT
     */
    //    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    //    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    /*
     * Install the TSS into the GDT
     */
    //   tss_install(5, 0x10, 0x0);

    /*
     * Flush our the old GDT / TSS and install the new changes!
     */
    kprintf("Flushing GDT\n");
    gdt_dump();
    gdt_flush();
    //   _tss_flush();
}
