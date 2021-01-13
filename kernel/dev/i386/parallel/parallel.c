//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/parallel/parallel.h>
#include <sys/asm/asm.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_parallel.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>

#define PARALLEL_DEVICE_LTP1_BASE 0x378
#define PARALLEL_DEVICE_LTP2_BASE 0x278
#define PARALLEL_DEVICE_LTP3_BASE 0x3BC
#define PARALLEL_DEVICE_LTP1_IRQ 7
#define PARALLEL_DEVICE_LTP2_IRQ 6
#define PARALLEL_DEVICE_LTP3_IRQ 5

// https://wiki.osdev.org/Parallel_port

struct parallel_devicedata {
    uint16_t address;
    uint16_t irq;
} __attribute__((packed));

void parallel_irq_handler(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
void parallel_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct parallel_devicedata* deviceData = (struct parallel_devicedata*)(dev->deviceData);
    kprintf("Init %s at IRQ %llu Base %#hX (%s)\n", dev->description, deviceData->irq, deviceData->address, dev->name);
    interrupt_router_register_interrupt_handler(deviceData->irq, &parallel_irq_handler);
}

void parallel_devicemgr_register_device(uint64_t base, uint8_t irq) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "Parallel Port");
    deviceinstance->devicetype = PARALLEL;
    deviceinstance->init = &parallel_device_init;
    /*
     * device api
     */
    struct deviceapi_parallel* api = (struct deviceapi_parallel*)kmalloc(sizeof(struct deviceapi_parallel));
    deviceinstance->api = api;
    /*
     * device data
     */
    struct parallel_devicedata* deviceData = (struct parallel_devicedata*)kmalloc(sizeof(struct parallel_devicedata));
    deviceData->address = base;
    deviceData->irq = irq;
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

void parallel_devicemgr_register_devices() {
    parallel_devicemgr_register_device(PARALLEL_DEVICE_LTP1_BASE, PARALLEL_DEVICE_LTP1_IRQ);
}