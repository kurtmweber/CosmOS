//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/x86-64/gdt/tss.h>

#define TSS_ENTRY 5

//   mov rax, 0x2B     ; Load the index of our TSS structure - The index is
//                     ; 0x28, as it is the 5th selector and each is 8 bytes
//                     ; long, but we set the bottom two bits (making 0x2B)
//                     ; so that it has an RPL of 3, not zero.
//   ltr ax            ; Load 0x2B into the task state register.
//   ret

void tss_flush() {
    asm volatile("mov %rax, 0x2b");
    asm volatile("mov ltr, %ax");
    asm volatile("ret");
}