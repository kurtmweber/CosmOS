//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/fs/testdfs.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/console/console.h>
#include <sys/fs/fs.h>

void test_dfs() {
	uint8_t devicename[] ={"disk2"};
	uint8_t fsname[] ={"dfs"};

	struct device* dsk = devicemgr_find_device(devicename);
	if (0!=dsk){
		struct fs_filesystem* fs = fs_find(fsname);
		if (0!=fs){
			(*fs->format)(dsk);
			(*fs->list_dir)(dsk);
		} else {
			kprintf("Unable to find %s\n",fsname);
		}
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}


