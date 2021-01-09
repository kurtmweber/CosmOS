//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testcfs.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/console/console.h>
#include <sys/fs/cfs/cfs.h>

void test_cfs() {
	// get virtual block device
	uint8_t devicename[] ={"disk1"};

	struct device* dsk = devicemgr_find_device(devicename);
	if (0!=dsk){
        cfs_format(dsk);
//        struct list* lst = list_new();
 //       cfs_list_dir(dsk, lst);
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}


