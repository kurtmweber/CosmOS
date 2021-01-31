
#ifndef _MISC_H
#define _MISC_H

#include <sys/x86-64/mm/mm.h>

void asm_cli();
void asm_hlt();
void asm_sti();
void* asm_cr2_read();
pttentry asm_cr3_read();
void asm_cr3_reload();

#endif