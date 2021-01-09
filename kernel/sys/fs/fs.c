//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/fs.h>
#include <sys/fs/cfs/cfs.h>
#include <sys/fs/fat/fat.h>
#include <sys/fs/sfs/sfs.h>
#include <sys/debug/assert.h>
#include <sys/console/console.h>
#include <sys/string/string.h>

struct list* fs_list;

void fs_init() {
    kprintf("Initializing filesystems\n");
    fs_list = list_new();
    sfs_register();
    cfs_register();
    fat_register();
}

void fs_register(struct filesystem* fs) {
    ASSERT_NOT_NULL(fs, "fs cannot be null");
    ASSERT_NOT_NULL(fs->format, "format cannot be null");
    ASSERT_NOT_NULL(fs->list_dir, "list_dir cannot be null");
    ASSERT_NOT_NULL(fs->name, "name cannot be null");
    kprintf("Registering filesystem %s\n",(*fs->name)());
    list_add(fs_list, fs);
}

struct filesystem* fs_find(uint8_t* name) {
    ASSERT_NOT_NULL(name, "name cannot be null");
    for (uint8_t i=0; i< list_count(fs_list);i++){
        struct filesystem* fs = (struct filesystem*) list_get(fs_list,i);
        if (strcmp(name, (*fs->name)())==0){
            return fs;
        }
    }
    return 0;
}