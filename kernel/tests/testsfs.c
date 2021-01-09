//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testsfs.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>
#include <sys/fs/fs.h>

void test_sfs() {
	// get virtual block device
	uint8_t devicename[] ={"disk2"};
	uint8_t fsname[] ={"sfs"};

	struct device* dsk = devicemgr_find_device(devicename);
	if (0!=dsk){
		struct filesystem* fs = fs_find(fsname);
		if (0!=fs){
			kprintf("Formatting %s to %s\n",devicename, fsname);
			(*fs->format)(dsk);
			kprintf("Done\n");
		} else {
			kprintf("Unable to find %s\n",fsname);
		}
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}
