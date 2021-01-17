//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/tfs/tfs.h>
#include <sys/console/console.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/string/string.h>
#include <tests/fs/test_tfs.h>

uint8_t FILE1_CONTENT[] = {"This is the file i am i am, this is the file i am"};
uint8_t FILE1_NAME[] = {"Dave"};

void test_dfs() {
    uint8_t devicename[] = {"disk1"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        tfs_attach(dsk);
        tfs_detach(dsk);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
