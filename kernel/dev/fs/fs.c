//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/cfs/cfs.h>
#include <dev/fs/fat/fat.h>
#include <dev/fs/fs.h>
#include <dev/fs/sfs/sfs.h>
#include <dev/fs/tfs/tfs.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>

struct arraylist* fs_list;

void fs_init() {
    kprintf("Registering filesystems\n");
    fs_list = arraylist_new();
    sfs_register();
    cfs_register();
    fat_register();
    tfs_register();
}

void fs_register(struct fs_filesystem* fs) {
    ASSERT_NOT_NULL(fs);
    ASSERT_NOT_NULL(fs->format);
    ASSERT_NOT_NULL(fs->list_dir);
    ASSERT_NOT_NULL(fs->name);
    //   kprintf("Registering filesystem %s\n",(*fs->name)());
    arraylist_add(fs_list, fs);
}

struct fs_filesystem* fs_find(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    for (uint8_t i = 0; i < arraylist_count(fs_list); i++) {
        struct fs_filesystem* fs = (struct fs_filesystem*)arraylist_get(fs_list, i);
        if (strcmp(name, (*fs->name)()) == 0) {
            return fs;
        }
    }
    return 0;
}

/*
 * make a new directory listing
 */
struct fs_directory_listing* fs_directory_listing_new() {
    struct fs_directory_listing* ret = (struct fs_directory_listing*)kmalloc(sizeof(struct fs_directory_listing));
    ret->lst = arraylist_new();
    return ret;
}

/*
 * ugh
 */
void fs_directory_listing_delete(struct fs_directory_listing* listing) {
    ASSERT_NOT_NULL(listing);
    ASSERT_NOT_NULL(listing->lst);

    for (uint32_t i; i < arraylist_count(listing->lst); i++) {
        struct fs_directory* dir = (struct fs_directory*)arraylist_get(listing->lst, i);
        if (0 != dir) {
            kfree(dir);
        }
    }
    kfree(listing);
}
