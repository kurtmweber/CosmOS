//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/fs.h>
#include <sys/fs/tfs/tfs_dir.h>
#include <tests/fs/test_sfs.h>

void test_sfs() {
    uint8_t devicename[] = {"disk2"};
    uint8_t fsname[] = {"sfs"};
    uint8_t filename[] = {"file1.txt"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        struct fs_filesystem* fs = fs_find(fsname);
        if (0 != fs) {
            kprintf("Formatting %s to %s\n", devicename, fsname);
            (*fs->format)(dsk);
            kprintf("Done\n");

            uint64_t id = tfs_dir_find_file(dsk, filename);
            //	if (0==id){
            //	}
        } else {
            kprintf("Unable to find %s\n", fsname);
        }
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
