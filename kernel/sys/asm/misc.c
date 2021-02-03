/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/x86-64/mm/mm.h>
#include <types.h>

#ifdef TARGET_PLATFORM_i386

void asm_hlt() {
    asm volatile("hlt");

    return;
}

void asm_cli() {
    asm volatile("cli");

    return;
}

void* asm_cr2_read() {
    void* ret;

    asm volatile("mov %%cr2, %0" : "=r"(ret));

    return ret;
}

pttentry asm_cr3_read() {
    pttentry cr3;

    asm volatile("mov %%cr3 ,%0" : "=r"(cr3));

    return cr3;
}

void asm_cr3_reload() {
    asm volatile("mov %%cr3, %%rax\n"
                 "mov %%rax, %%cr3"
                 :
                 :
                 : "%eax");

    return;
}

void asm_sti() {
    asm volatile("sti");

    return;
}

#endif