//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/fat/fat.h>
#include <sys/console/console.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <tests/fs/test_fat.h>

void test_fat() {
    uint8_t devicename[] = {"disk1"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        fat_attach(dsk);
        fat_detach(dsk);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
