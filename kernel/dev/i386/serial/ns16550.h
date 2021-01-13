//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// common definition of a National Semiconductor 16550 port

#ifndef _NS16550_H
#define _NS16550_H

#include <types.h>

#define COM1_ADDRESS (uint16_t)0x3F8
#define COM2_ADDRESS (uint16_t)0x2F8
#define COM3_ADDRESS (uint16_t)0x3E8
#define COM4_ADDRESS (uint16_t)0x2E8

#define SERIAL_IRQ1 (uint8_t)0x3
#define SERIAL_IRQ2 (uint8_t)0x4

// when we do want to receive interrupts?
#define SERIAL_INTERRUPT_DATA_AVAILABLE 0x01
#define SERIAL_INTERRUPT_TRANSMITTER_EMPTY 0x02
#define SERIAL_INTERRUPT_BREAK_ERROR 0x04
#define SERIAL_INTERRUPT_STATUS_CHANGE 0x08

// character length
#define SERIAL_CHARACTER_LENGTH_5 0x00
#define SERIAL_CHARACTER_LENGTH_6 0x01
#define SERIAL_CHARACTER_LENGTH_7 0x20
#define SERIAL_CHARACTER_LENGTH_8 0x03

// stop bits
#define SERIAL_STOP_BITS_1 0x00
#define SERIAL_STOP_BITS_1_5 0x04

// parity
#define SERIAL_PARITY_NONE 0x00
#define SERIAL_PARITY_ODD 0x08
#define SERIAL_PARITY_EVEN 0x18
#define SERIAL_PARITY_MARK 0x28
#define SERIAL_PARITY_SPACE 0x38

// line status
#define SERIAL_LINE_STATUS_DATA_READY 0x01
#define SERIAL_LINE_STATUS_OVERRUN_ERROR 0x02
#define SERIAL_LINE_STATUS_PARITY_ERROR 0x04
#define SERIAL_LINE_STATUS_FRAMING_ERROR 0x08
#define SERIAL_LINE_STATUS_BREAK 0x10
#define SERIAL_LINE_STATUS_TRANSMIT_HOLDING_EMPTY 0x20
#define SERIAL_LINE_STATUS_TRANSMIT_EMPTY 0x40
#define SERIAL_LINE_STATUS_IMPENDING_ERROR 0x80

struct rs232_16550 {
    uint8_t data;
    uint8_t interrupt;
    uint8_t fifocontrol;
    uint8_t linecontrol;
    uint8_t modemcontrol;
    uint8_t linestatus;
    uint8_t modemstatus;
    uint8_t scratch;
} __attribute__((packed));

#endif