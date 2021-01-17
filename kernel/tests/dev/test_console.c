//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/console/serial_console.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_console.h>
#include <sys/devicemgr/devicemgr.h>
#include <tests/dev/test_bda.h>

void test_console_dev() {
    uint8_t devicename[] = {"serial0"};

    /*
     * find the physical disk
     */
    struct device* serial = devicemgr_find_device(devicename);
    if (0 != serial) {
        // attach the console
        struct device* console_device = serial_console_attach(serial);

        struct deviceapi_console* console_api = (struct deviceapi_console*)console_device->api;

        (*console_api->write)(console_device, "Console test\n");

        // detach the console
        serial_console_detach(console_device);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
