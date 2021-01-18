//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/console/vga_console.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_console.h>
#include <sys/devicemgr/devicemgr.h>

void test_vga_console_dev() {
    uint8_t devicename[] = {"vga0"};

    /*
     * find the physical disk
     */
    struct device* vga = devicemgr_find_device(devicename);
    if (0 != vga) {
        // attach the console
        struct device* console_device = vga_console_attach(vga);

        struct deviceapi_console* console_api = (struct deviceapi_console*)console_device->api;

        (*console_api->write)(console_device, "Console test\n");

        // detach the console
        vga_console_detach(console_device);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
