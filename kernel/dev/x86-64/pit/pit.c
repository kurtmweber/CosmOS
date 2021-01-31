/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/pit/pit.h>
#include <sys/asm/asm.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_pit.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

// https://wiki.osdev.org/Programmable_Interval_Timer
// http://www.osdever.net/bkerndev/Docs/pit.htm

#define PIT_IRQ 0x00

#define PIT_PORT_0 0x40
#define PIT_PORT_1 0x41
#define PIT_PORT_2 0x42
#define PIT_PORT_COMMAND 0x43

struct arraylist* pitEvents;

uint64_t tickcount = 0;

#define PIT_HZ 1  // 10 interrupts per second

// This is the perfect place to handle context switches.  Just saying.
void pit_handle_irq(stackFrame* frame) {
    ASSERT_NOT_NULL(pitEvents);
    ASSERT_NOT_NULL(frame);
    //  kprintf("@");
    tickcount = tickcount + 1;
}

/*
 * perform device instance specific init here
 */
/*
 * According to: https://wiki.osdev.org/Programmable_Interval_Timer
 *
 * "Typically during boot the BIOS sets channel 0 with a count of 65535
 *  or 0 (which translates to 65536), which gives an output frequency of
 *  18.2065 Hz (or an IRQ every 54.9254 ms)"
 */
uint8_t pit_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s at IRQ %llu (%s)\n", dev->description, PIT_IRQ, dev->name);
    interrupt_router_register_interrupt_handler(PIT_IRQ, &pit_handle_irq);
    return 1;
}

uint64_t pit_tickcount(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    return tickcount;
}

void pit_subscribe(pit_event event) {
    ASSERT_NOT_NULL(pitEvents);
    ASSERT_NOT_NULL(event);
    arraylist_add(pitEvents, event);
}

void pit_devicemgr_register_devices() {
    pitEvents = arraylist_new();
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "8253/8254 PIT");
    deviceinstance->devicetype = PIT;
    deviceinstance->init = &pit_init;
    /*
     * device api
     */
    struct deviceapi_pit* api = (struct deviceapi_pit*)kmalloc(sizeof(struct deviceapi_pit));
    api->tickcount = &pit_tickcount;
    api->subscribe = &pit_subscribe;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}
