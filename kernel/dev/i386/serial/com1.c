//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/serial/com1.h>
#include <dev/i386/serial/ns16550.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>

int com1_is_transmit_empty() {
    struct rs232_16550* comport = (struct rs232_16550*)COM1_ADDRESS;
    uint8_t data = asm_in_b((uint64_t) & (comport->linestatus));
    return data & 0x20;
}

void com1_serial_write_char(const uint8_t c) {
    struct rs232_16550* comport = (struct rs232_16550*)COM1_ADDRESS;

    while (com1_is_transmit_empty() == 0)
        ;
    asm_out_b((uint64_t) & (comport->data), c);
}

void com1_serial_write(const uint8_t* c) {
    ASSERT_NOT_NULL(c);
    uint16_t i = 0;
    while (c[i] != 0) {
        com1_serial_write_char(c[i++]);
    }
}
