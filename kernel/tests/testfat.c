//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testfat.h>
#include <sys/fs/fat/fat.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>
#include <sys/collection/list/list.h>

void test_fat() {
    const uint8_t device[] = {"disk1"};
	struct device* disk = devicemgr_find_device(device);
    if (0!=disk){
        struct list* lst = list_new();

        fat_list_dir(disk, lst);
        

//        sfs_format(disk);
    } else {
        kprintf("Unable to find %s\n",device);
    }
}
