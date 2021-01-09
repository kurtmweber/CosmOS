//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testfat.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>
#include <sys/collection/list/list.h>
#include <sys/fs/fs.h>

void test_fat() {
	// get virtual block device
	uint8_t devicename[] ={"disk1"};
	uint8_t fsname[] ={"fat"};

	struct device* dsk = devicemgr_find_device(devicename);
	if (0!=dsk){

		struct filesystem* fs = fs_find(fsname);
		if (0!=fs){
            struct list* lst = list_new();

			(*fs->list_dir)(dsk, lst);
		} else {
			kprintf("Unable to find %s\n",fsname);
		}
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}
