//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/partition_table/mbr_partition_table.h>
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

        uint8_t number_partitions = (*api->partitions)(dev);
        kprintf("number_partitions %llu\n", number_partitions);

        uint64_t lba = (*api->lba)(dev, 0);
        uint64_t type = (*api->type)(dev, 0);
        kprintf("partition 0 type %llu lba %llu\n", type, lba);

        mbr_pt_detach(dev);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
