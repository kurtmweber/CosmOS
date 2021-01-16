//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
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
    uint32_t base;
} __attribute__((packed));

// GDT
struct gdt_entry gdt[GDT_SIZE];

// pointer to GDT
struct gdt_ptr gdt_pointer;

/* Setup a descriptor in the Global Descriptor Table */
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
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /*
     * Finally, set up the granularity and access flags
     */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
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
    gdt_set_gate(0, 0, 0, 0, 0);

    /*
     * The second entry is our Code Segment.  The base address
     * is 0, the limit is 4 gigabytes, it uses 4 kilobyte
     * granularity, uses 32-bit opcodes, and is a Code Segment
     * descriptor.  Please check the table above in the tutorial
     * in order to see exactly what each value means
     */
    // 10011010 = 0x9A
    // 10101111 = 0xAF
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF);

    /*
     * The third entry is our Data Segment.  It's exactly the
     * same as our code segment, but the descriptor type in
     * this entry's access byte says it's a Data Segment
     */
    // 10010010 = 0x92
    // 00000000 = 0
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0x00);

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
    gdt_flush();
    //   _tss_flush();
}

//   lgdt [gdt_pointer]

//   mov rax, 0x10
//   mov ds, rax
//   mov es, rax
//   mov fs, rax
//   mov gs, rax
//   mov ss, rax

//   lea rax, [rel .flush]
//   push 0x08
//   push rax
//  retf
//.flush : ret

inline void gdt_flush() {
    __asm__ __volatile__("lgdt %0" ::"m"(gdt_pointer));
}