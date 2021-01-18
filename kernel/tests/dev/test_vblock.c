//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_vblock.h>

void test_vblock() {
    // get virtual block device
    uint8_t devicename[] = {"vblock0"};

    struct device* ata = devicemgr_find_device(devicename);
    if (0 != ata) {
        struct deviceapi_block* ata_api = (struct deviceapi_block*)ata->api;

        uint8_t data[256];
        memset((uint8_t*)data, 0, 255 * sizeof(uint8_t));
        (*ata_api->read)(ata, 0, data, 255);
        debug_show_memblock(data, 32);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
