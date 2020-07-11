/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ASM_H
#define _ASM_H

#include <types.h>
#include <mm/mm.h>

#include "io.c"

#ifndef _ASM_C
void asm_cli();
void asm_hlt();
void asm_sti();
pttentry asm_cr3_read();
#endif

#ifndef _IO_C
/*uint8_t asm_in_b(uint16_t port);
uint32_t asm_in_d(uint16_t port);
uint16_t asm_in_w(uint16_t port);
void asm_out_b(uint16_t port, uint8_t data);
void asm_out_d(uint16_t port, uint32_t data);
void asm_out_w(uint16_t port, uint16_t data);*/
#endif

#endif