//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/console/console.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/fs.h>
#include <tests/fs/test_fat.h>

void test_fat() {
    uint8_t devicename[] = {"disk1"};
    uint8_t fsname[] = {"fat"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        struct fs_filesystem* fs = fs_find(fsname);
        if (0 != fs) {
            struct fs_directory_listing* listing = (*fs->list_dir)(dsk);
            kprintf("size %llu\n", arraylist_count(listing->lst));

            for (uint32_t i = 0; i < arraylist_count(listing->lst); i++) {
                struct fs_file* file = (struct fs_file*)arraylist_get(listing->lst, i);
                kprintf("%llu %s %llu\n", i, file->name, file->size);
            }
            fs_directory_listing_delete(listing);
        } else {
            kprintf("Unable to find %s\n", fsname);
        }
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
