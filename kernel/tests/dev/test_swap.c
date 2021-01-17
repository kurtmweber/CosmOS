//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/swap/swap.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_ramdisk.h>

void test_swap() {
    uint8_t devicename[] = {"disk1"};

    /*
     * find the physical disk
     */
    struct device* ata = devicemgr_find_device(devicename);
    if (0 != ata) {
        struct deviceapi_block* ata_api = (struct deviceapi_block*)ata->api;
    } else {
        kprintf("Unable to find %s\n", devicename);
    }

    // attach the swap
    struct device* swap_device = swap_attach(ata);

    // detach the ramdisk
    swap_detach(swap_device);
}
