//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/pt/mbr_pt.h>
#include <sys/deviceapi/deviceapi_part_table.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/fs/test_mbr.h>

void test_mbr() {
    uint8_t devicename[] = {"disk1"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        struct device* dev = mbr_pt_attach(dsk);

        struct deviceapi_part_table* api = (struct deviceapi_part_table*)dev->api;

        uint8_t number_partitions = (*api->parititions)(dev);
        kprintf("number_partitions %llu\n", number_partitions);
        mbr_pt_detach(dev);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
