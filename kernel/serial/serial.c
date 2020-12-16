//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <serial/serial.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>

#define COM1_ADDRESS 0x3F8
#define COM2_ADDRESS 0x2F8
#define COM3_ADDRESS 0x3E8
#define COM4_ADDRESS 0x2E8

#define SERIAL_IRQ1 3
#define SERIAL_IRQ2 4

void serial_irq_handler(){

}

int is_transmit_empty() {
   return asm_in_b(COM1_ADDRESS + 5) & 0x20;
}

void serial_write_char(uint8_t c){
   while (is_transmit_empty() == 0);
   asm_out_b(COM1_ADDRESS,c);
}

void serial_write(uint8_t* c){
    uint16_t i =0;
    while(c[i]!=0){
        serial_write_char(c[i++]);
    }
}

// https://wiki.osdev.org/Serial_Ports
void init_port(uint16_t portAddress) {
   asm_out_b(portAddress + 1, 0x00);    // Disable all interrupts
   asm_out_b(portAddress + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   asm_out_b(portAddress + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   asm_out_b(portAddress + 1, 0x00);    //                  (hi byte)
   asm_out_b(portAddress + 3, 0x03);    // 8 bits, no parity, one stop bit
   asm_out_b(portAddress + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   asm_out_b(portAddress + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

void serial_init() {
    init_port(COM1_ADDRESS);
	registerInterruptHandler(SERIAL_IRQ1, &serial_irq_handler);
	registerInterruptHandler(SERIAL_IRQ2, &serial_irq_handler);
}
