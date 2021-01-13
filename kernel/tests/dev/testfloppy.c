//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_floppy.h>
#include <tests/dev/testfloppy.h>

void floppyread() {
    // get the floppy
    struct device* floppy = devicemgr_find_device("floppy0");
    if (0 != floppy) {
        struct deviceapi_floppy* floppy_api = (struct deviceapi_floppy*)floppy->api;

        uint8_t data[256];
        (*floppy_api->read)(floppy, 0, data, 255);
        debug_show_memblock(data, 32);
    } else {
        kprintf("Unable to find floppy0\n");
    }
}
