//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/ramdisk/ramdisk.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_ramdisk.h>

#define RAMDISK_SECTOR_SIZE 512
#define RAMDISK_TOTAL_SECTORS 1000

uint8_t* testdata =
    "We were very tired, we were very merry, \
We had gone back and forth all night on the ferry. \
We hailed, Good morrow, mother! to a shawl-covered head, \
And bought a morning paper, which neither of us read; \
And she wept, God bless you! for the apples and pears, \
And we gave her all our money but our subway fares.";

void test_ramdisk() {
    // attach the ramdisk
    struct device* ramdisk_device = ramdisk_attach(RAMDISK_SECTOR_SIZE, RAMDISK_TOTAL_SECTORS);

    struct device* ata = devicemgr_find_device(ramdisk_device->name);
    if (0 != ata) {
        struct deviceapi_block* ata_api = (struct deviceapi_block*)ata->api;

        uint8_t data[256];
        memset((uint8_t*)data, 0, 255 * sizeof(uint8_t));

        (*ata_api->write)(ata, 7, testdata, strlen(testdata));

        uint8_t readdata[255];
        memset((uint8_t*)readdata, 0, 255 * sizeof(uint8_t));

        (*ata_api->read)(ata, 7, readdata, 512);

        debug_show_memblock((uint8_t*)readdata, 32);
        ASSERT(readdata[0] == 'W');
        ASSERT(strlen(readdata) == strlen(testdata));
    } else {
        kprintf("Unable to find %s\n", ramdisk_device->name);
    }

    // attach the ramdisk
    ramdisk_detach(ramdisk_device);
}
