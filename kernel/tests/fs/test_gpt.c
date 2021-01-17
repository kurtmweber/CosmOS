//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/fs/fs.h>
#include <sys/fs/guid_pt.h>
#include <tests/fs/test_gpt.h>

void test_gpt() {
    uint8_t devicename[] = {"disk1"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        struct guid_pt_header header;
        guid_pt_read_guid_pt_header(dsk, &header);
        kprintf("GPT paritions: %llu\n", header.num_partitions);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
