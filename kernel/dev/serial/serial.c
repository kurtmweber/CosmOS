//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/serial/serial.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <devicemgr/deviceapi/deviceapi_serial.h>
#include <debug/assert.h>
#include <console/console.h>
#include <collection/ringbuffer/ringbuffer.h>
#include <dev/serial/ns16550.h>

struct serial_devicedata {
    uint8_t irq;
    uint16_t address;
    struct ringbuffer* buffer;

} __attribute__((packed));

int serial_is_transmit_empty() {
    struct rs232_16550* comport = (struct rs232_16550*) COM1_ADDRESS;
    uint8_t data = asm_in_b((uint64_t)&(comport->linestatus));
    return data & 0x20;
}

void serial_write_char(const uint8_t c){
    struct rs232_16550* comport = (struct rs232_16550*) COM1_ADDRESS;

    while (serial_is_transmit_empty() == 0);
    asm_out_b((uint64_t) &(comport->data),c);
}

void serial_irq_handler(stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
    struct rs232_16550* comport = (struct rs232_16550*) COM1_ADDRESS;
    uint8_t data = asm_in_b((uint64_t)&(comport->data));

    // echo the data
    serial_write_char(data);
}

void serial_write_string(const uint8_t* c){
    uint16_t i =0;
    while(c[i]!=0){
        serial_write_char(c[i++]);
    }
}

// https://wiki.osdev.org/Serial_Ports
void serial_init_port(uint64_t portAddress) {

    struct rs232_16550* comport = (struct rs232_16550*) portAddress;

    // enable interrupt on received data
    asm_out_b((uint64_t)&(comport->interrupt),0x01);

    // set 38400 baud
    asm_out_b((uint64_t)&(comport->linecontrol),0x80);
    asm_out_b((uint64_t)&(comport->data),0x03);
    asm_out_b((uint64_t)&(comport->interrupt),0x00);
    asm_out_b((uint64_t)&(comport->linecontrol),0x03);

    // FIFO on
    asm_out_b((uint64_t)&(comport->fifocontrol),0xC7);

    // IRQ on, RTD/DSR set
    asm_out_b((uint64_t)&(comport->modemcontrol),0x0B);
}

/*
* perform device instance specific init here
*/
void serial_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    struct serial_devicedata* deviceData = (struct serial_devicedata*) dev->deviceData;
    kprintf("Init %s at IRQ %llu Base %#hX (%s)\n",dev->description, deviceData->irq, deviceData->address, dev->name);
    interrupt_router_register_interrupt_handler(deviceData->irq, &serial_irq_handler);
    serial_init_port(deviceData->address);
}

void serial_write(struct device* dev, const int8_t* c) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
   serial_write_string(c);
}

void serial_register_device(uint8_t irq, uint64_t base) {
    /*
    * ISA serial port specific data
    */
    struct serial_devicedata* deviceData = kmalloc(sizeof(struct serial_devicedata));
    deviceData->irq=irq;
    deviceData->address=base;
    deviceData->buffer = ringbuffer_new();
    /*
    * the device instance
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &serial_device_init;
    deviceinstance->deviceData = deviceData;
    deviceinstance->devicetype = SERIAL;
    devicemgr_set_device_description(deviceinstance, "RS232");
    /*
    * the device api
    */
    struct deviceapi_serial* api = (struct deviceapi_serial*) kmalloc(sizeof(struct deviceapi_serial));
    api->write = &serial_write;
    deviceinstance->api = api;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);  
}

/**
* find all RS232 devices and register them
*/
void serial_devicemgr_register_devices() {
    serial_register_device(SERIAL_IRQ2,COM1_ADDRESS);

    // TODO add code to check if these even exist
//    registerRS232Device(SERIAL_IRQ1,COM2_ADDRESS);
//    registerRS232Device(SERIAL_IRQ2,COM3_ADDRESS);
//    registerRS232Device(SERIAL_IRQ1,COM4_ADDRESS);
}
