//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <serial/serial.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>

#define COM1_ADDRESS (uint16_t) 0x3F8
#define COM2_ADDRESS (uint16_t) 0x2F8
#define COM3_ADDRESS (uint16_t) 0x3E8
#define COM4_ADDRESS (uint16_t) 0x2E8

#define SERIAL_IRQ1 (uint8_t) 0x3
#define SERIAL_IRQ2 (uint8_t) 0x4

struct comport {
    uint8_t irq;
    uint16_t address;
} __attribute__((packed));

struct rs232_16550 {
    uint8_t data;
    uint8_t interrupt;
    uint8_t fifocontrol;
    uint8_t linecontrol;
    uint8_t modemcontrol;
    uint8_t linestatus;
    uint8_t modemstatus;
    uint8_t scratch;    
} __attribute__((packed)) rs232_16550_t;

int is_transmit_empty() {
    struct rs232_16550* com1 = (struct rs232_16550*) COM1_ADDRESS;
    uint8_t data = asm_in_b((uint64_t)&(com1->linestatus));
    return data & 0x20;
}

void serial_write_char(const uint8_t c){
    struct rs232_16550* com1 = (struct rs232_16550*) COM1_ADDRESS;

    while (is_transmit_empty() == 0);
    asm_out_b((uint64_t) &(com1->data),c);
}

void serial_irq_handler(stackFrame *frame){
    struct rs232_16550* com1 = (struct rs232_16550*) COM1_ADDRESS;
    uint8_t data = asm_in_b((uint64_t)&(com1->data));

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
void init_port(uint64_t portAddress) {

    struct rs232_16550* com1 = (struct rs232_16550*) portAddress;

    // enable interrupt on received data
    asm_out_b((uint64_t)&(com1->interrupt),0x01);

    // set 38400 baud
    asm_out_b((uint64_t)&(com1->linecontrol),0x80);
    asm_out_b((uint64_t)&(com1->data),0x03);
    asm_out_b((uint64_t)&(com1->interrupt),0x00);
    asm_out_b((uint64_t)&(com1->linecontrol),0x03);

    // FIFO on
    asm_out_b((uint64_t)&(com1->fifocontrol),0xC7);

    // IRQ on, RTD/DSR set
    asm_out_b((uint64_t)&(com1->modemcontrol),0x0B);
}

/*
* perform device instance specific init here
*/
void deviceInitCOM1(struct device* dev){
    struct comport* cp = (struct comport*) dev->deviceData;
    kprintf("Init %s at IRQ %llu\n",dev->description, cp->irq);
    registerInterruptHandler(cp->irq, &serial_irq_handler);
    init_port(cp->address);
}

/**
* find all RS232 devices and register them
*/
void serial_register_devices() {
    // COM1
    struct comport* cp1 = kmalloc(sizeof(struct comport));
    cp1->irq=SERIAL_IRQ2;
    cp1->address=COM1_ADDRESS;
    struct device* deviceinstance1 = newDevice();
    deviceinstance1->init =  &deviceInitCOM1;
    deviceinstance1->deviceData = cp1;
    deviceinstance1->devicetype = SERIAL;
    deviceSetDescription(deviceinstance1, "RS232");
    registerDevice(deviceinstance1);

    // COM2
    struct comport* cp2 = kmalloc(sizeof(struct comport));
    cp2->irq=SERIAL_IRQ1;
    cp2->address=COM2_ADDRESS;
    struct device* deviceinstance2 = newDevice();
    deviceinstance2->init =  &deviceInitCOM1;
    deviceinstance2->deviceData = cp2;
    deviceinstance2->devicetype = SERIAL;
    deviceSetDescription(deviceinstance2, "RS232");
    registerDevice(deviceinstance2);

    // COM3
    struct comport* cp3 = kmalloc(sizeof(struct comport));
    cp3->irq=SERIAL_IRQ2;
    cp3->address=COM3_ADDRESS;
    struct device* deviceinstance3 = newDevice();
    deviceinstance3->init =  &deviceInitCOM1;
    deviceinstance3->deviceData = cp3;
    deviceinstance3->devicetype = SERIAL;
    deviceSetDescription(deviceinstance3, "RS232");
    registerDevice(deviceinstance3);

    // COM4
    struct comport* cp4 = kmalloc(sizeof(struct comport));
    cp4->irq=SERIAL_IRQ1;
    cp4->address=COM4_ADDRESS;
    struct device* deviceinstance4 = newDevice();
    deviceinstance4->init =  &deviceInitCOM1;
    deviceinstance4->deviceData = cp4;
    deviceinstance4->devicetype = SERIAL;
    deviceSetDescription(deviceinstance4, "RS232");
    registerDevice(deviceinstance4);
}
