//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/cfs/cfs.h>
#include <sys/console/console.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <tests/fs/test_cfs.h>

void test_cfs() {
    uint8_t devicename[] = {"disk1"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        cfs_attach(dsk);
        cfs_detach(dsk);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
