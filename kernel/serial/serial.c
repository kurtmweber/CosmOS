//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <serial/serial.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <device/device.h>
#include <console/console.h>

#define COM1_ADDRESS 0x3F8
#define COM2_ADDRESS 0x2F8
#define COM3_ADDRESS 0x3E8
#define COM4_ADDRESS 0x2E8

#define SERIAL_IRQ1 3
#define SERIAL_IRQ2 4

struct rs232_16550 {
    uint8_t data;
    uint8_t interrupt;
    uint8_t fifocontrol;
    uint8_t linecontrol;
    uint8_t modemcontrol;
    uint8_t linestatus;
    uint8_t modemstatus;
    uint8_t scratch;    
} rs232_16550_t;

int is_transmit_empty() {
    struct rs232_16550* com1 = (struct rs232_16550*) COM1_ADDRESS;
    uint8_t data = asm_in_b((uint16_t)&(com1->linestatus));
    return data & 0x20;
}

void serial_write_char(const uint8_t c){
    struct rs232_16550* com1 = (struct rs232_16550*) COM1_ADDRESS;

    while (is_transmit_empty() == 0);
    asm_out_b((uint16_t) &(com1->data),c);
}

void serial_irq_handler(){
    struct rs232_16550* com1 = (struct rs232_16550*) COM1_ADDRESS;
    uint8_t data = asm_in_b((uint16_t)&(com1->data));

    // echo the data
    serial_write_char(data);
}

void serial_write(const uint8_t* c){
    uint16_t i =0;
    while(c[i]!=0){
        serial_write_char(c[i++]);
    }
}

// https://wiki.osdev.org/Serial_Ports
void init_port(uint16_t portAddress) {

    struct rs232_16550* com1 = (struct rs232_16550*) portAddress;

    // enable interrupt on received data
    asm_out_b((uint16_t)&(com1->interrupt),0x01);

    // set 38400 baud
    asm_out_b((uint16_t)&(com1->linecontrol),0x80);
    asm_out_b((uint16_t)&(com1->data),0x03);
    asm_out_b((uint16_t)&(com1->interrupt),0x00);
    asm_out_b((uint16_t)&(com1->linecontrol),0x03);

    // FIFO on
    asm_out_b((uint16_t)&(com1->fifocontrol),0xC7);

    // IRQ on, RTD/DSR set
    asm_out_b((uint16_t)&(com1->modemcontrol),0x0B);
}

/*
* perform device instance specific init here
*/
void deviceInitCOM1(struct device* dev){
    kprintf("Init COM1\n");
    init_port(COM1_ADDRESS);
}

/**
* find all RS232 devices and register them
*/
void serial_register_devices() {
    struct device* com1deviceinstance = newDevice();
    com1deviceinstance->init =  &deviceInitCOM1;
    registerDevice(com1deviceinstance);

	registerInterruptHandler(SERIAL_IRQ1, &serial_irq_handler);
	registerInterruptHandler(SERIAL_IRQ2, &serial_irq_handler);
}
