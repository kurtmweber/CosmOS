//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testsfs.h>
#include <sfs/sfs.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>

void test_sfs() {
	struct device* disk = devicemgr_findDevice("disk0");
    if (0!=disk){
        sfs_format(disk);
    } else {
        kprintf("Unable to find disk0\n");
    }
}
