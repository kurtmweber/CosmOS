//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/bda/bda.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_bda.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>

#define BDA_ADDRESS 0x400

struct bda {
    uint16_t serial_port_addresses[4];
    uint16_t parallel_port_addresses[3];
    uint16_t ebda;
    uint16_t hwflags;
    uint16_t kb_before_ebda;
    uint16_t keyboard_state;
    uint8_t keyboard_buffer[32];
    uint8_t display_mode;
    uint16_t textmode_columns;
    uint16_t video_port_address;
    uint16_t irq0_ticks_since_boot;
    uint8_t number_hd;
    uint16_t keyboard_buffer_start;
    uint16_t keyboard_buffer_end;
    uint8_t keyboard_last;
} __attribute__((packed));

uint64_t bda_get_ebda_address() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return (data_area->ebda) << 4;
}

uint16_t bda_text_columns() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->textmode_columns;
}

uint8_t bda_display_mode() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->display_mode;
}

uint16_t bda_uptime() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->irq0_ticks_since_boot;
}

uint16_t bda_video_port() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->video_port_address;
}

uint8_t bda_num_hd() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->number_hd;
}

uint16_t bda_serial0_base() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->serial_port_addresses[0];
}

uint16_t bda_serial1_base() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->serial_port_addresses[1];
}

uint16_t bda_serial2_base() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->serial_port_addresses[2];
}

uint16_t bda_serial3_base() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->serial_port_addresses[3];
}

uint16_t bda_parallel0_base() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->parallel_port_addresses[0];
}

uint16_t bda_parallel1_base() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->parallel_port_addresses[1];
}

uint16_t bda_parallel2_base() {
    struct bda* data_area = (struct bda*)BDA_ADDRESS;
    return data_area->parallel_port_addresses[2];
}

/*
 * perform device instance specific init here
 */
uint8_t bda_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

void bda_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "BIOS Data Area");
    deviceinstance->devicetype = BDA;
    deviceinstance->init = &bda_device_init;
    /*
     * api
     */
    //   struct deviceapi_bda* api = (struct deviceapi_bda*)kmalloc(sizeof(struct deviceapi_bda));
    //   deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}