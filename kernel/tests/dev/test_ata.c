//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_ata.h>

void test_ata1() {
    // get virtual block device
    uint8_t devicename[] = {"disk0"};

    struct device* ata = devicemgr_find_device(devicename);
    if (0 != ata) {
        struct deviceapi_block* ata_api = (struct deviceapi_block*)ata->api;

        uint16_t sector_size = (*ata_api->sector_size)(ata);
        kprintf("Sector size %llu\n", sector_size);

        uint32_t total_size = (*ata_api->total_size)(ata);
        kprintf("Total size %llu\n", total_size);

        uint32_t buffersize = sizeof(uint8_t) * sector_size;

        uint8_t* data = kmalloc(buffersize);
        memset(data, 0, buffersize);

        (*ata_api->read)(ata, 0, data, 1);  // 1 sector
        debug_show_memblock(data, sector_size);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}

void test_ata2() {
    // get virtual block device
    uint8_t devicename[] = {"disk0"};

    struct device* ata = devicemgr_find_device(devicename);
    if (0 != ata) {
        struct deviceapi_block* ata_api = (struct deviceapi_block*)ata->api;

        uint16_t sector_size = (*ata_api->sector_size)(ata);
        kprintf("Sector size %llu\n", sector_size);

        uint32_t total_size = (*ata_api->total_size)(ata);
        kprintf("Total size %llu\n", total_size);

        uint32_t buffersize = sizeof(uint8_t) * sector_size;

        uint8_t* outdata = kmalloc(buffersize);
        memset(outdata, 0x11, buffersize);

        // write
        //	debug_show_memblock(outdata, sector_size);
        (*ata_api->write)(ata, 0, outdata, 1);  // 1 sector

        // read
        uint8_t* indata = kmalloc(buffersize);
        memset(indata, 0, buffersize);
        (*ata_api->read)(ata, 0, indata, 1);  // 1 sector

        debug_show_memblock(indata, sector_size);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}

void test_ata() {
    test_ata1();
    //	test_ata2();
}
