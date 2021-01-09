//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testsfs.h>
#include <sys/fs/sfs/sfs.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>

void test_sfs() {
    uint8_t devicename[] ={"disk2"};

	struct device* disk = devicemgr_find_device(devicename);
    if (0!=disk){
        kprintf("formatting %s\n",devicename);
        sfs_format(disk);
    } else {
        kprintf("Unable to find %s\n",devicename);
    }
}
