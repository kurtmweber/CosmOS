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
#include <sys/deviceapi/deviceapi_swap.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_ramdisk.h>

const uint8_t SWAP_TEST_STRING[] = "We were very tired and we were very merry";

void test_swap() {
    uint8_t devicename[] = {"disk1"};

    /*
     * find the physical disk
     */
    struct device* ata = devicemgr_find_device(devicename);
    if (0 != ata) {
        // attach the swap
        struct device* swap_device = swap_attach(ata);

        struct deviceapi_swap* swap_api = (struct deviceapi_swap*)swap_device->api;

        uint16_t block_size = (*swap_api->block_size)(swap_device);
        kprintf("Swap device block size: %llu\n", block_size);

        // write the string
        uint8_t write_buffer[512];
        memset(write_buffer, 0, 512);
        strcpy(write_buffer, SWAP_TEST_STRING);
        kprintf("Str: %s\n", write_buffer);

        (*swap_api->write)(swap_device, write_buffer, 0);

        // read it back
        uint8_t read_buffer[512];
        (*swap_api->write)(swap_device, read_buffer, 0);

        kprintf("Str: %s\n", read_buffer);

        // detach the swap
        swap_detach(swap_device);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
