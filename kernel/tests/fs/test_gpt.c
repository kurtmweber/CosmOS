//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/partition_table/guid_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/fs/test_gpt.h>

void test_gpt_attach() {
    uint8_t devicename[] = {"disk1"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        struct device* dev = guid_pt_attach(dsk);
        guid_pt_detach(dev);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}

void test_gpt() {
    kprintf("Testing GPT GUID\n");

    uint8_t id[] = {0xA2, 0xA0, 0xD0, 0xEB, 0xE5, 0xB9, 0x33, 0x44, 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7};
    uint8_t str[255];
    guid_partition_type_to_string(id, str, 255);
    ASSERT(0 == strcmp(str, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7"));
    //   kprintf("s: %s\n", str);
}
