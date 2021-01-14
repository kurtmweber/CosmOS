//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/bda/bda.h>
#include <sys/asm/asm.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_bda.h>
#include <sys/devicemgr/devicemgr.h>

uint16_t ebda_get_rsdp() {
    uint16_t ebda_address = bda_get_ebda_address();
}

/*
 * perform device instance specific init here
 */
void ebda_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s) at %#hX\n", dev->description, dev->name, bda_get_ebda_address());
}

void ebda_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "Extended BIOS Data Area");
    deviceinstance->devicetype = EBDA;
    deviceinstance->init = &ebda_device_init;
    /*
     * api
     */
    struct deviceapi_bda* api = (struct deviceapi_bda*)kmalloc(sizeof(struct deviceapi_bda));
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}