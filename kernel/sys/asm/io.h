
#ifndef _IO_H
#define _IO_H

#include <types.h>

uint8_t asm_in_b(uint16_t port);
uint32_t asm_in_d(uint16_t port);
uint16_t asm_in_w(uint16_t port);
void asm_out_b(uint16_t port, uint8_t data);
void asm_out_d(uint16_t port, uint32_t data);
void asm_out_w(uint16_t port, uint16_t data);

#endif