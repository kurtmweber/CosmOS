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
#include <sys/fs/dfs/dfs.h>
#include <sys/debug/assert.h>
#include <sys/console/console.h>
#include <sys/string/string.h>
#include <sys/i386/mm/mm.h>

struct list* fs_list;

void fs_init() {
    kprintf("Registering filesystems\n");
    fs_list = list_new();
    sfs_register();
    cfs_register();
    fat_register();
    dfs_register();
}

void fs_register(struct fs_filesystem* fs) {
    ASSERT_NOT_NULL(fs, "fs cannot be null");
    ASSERT_NOT_NULL(fs->format, "format cannot be null");
    ASSERT_NOT_NULL(fs->list_dir, "list_dir cannot be null");
    ASSERT_NOT_NULL(fs->name, "name cannot be null");
 //   kprintf("Registering filesystem %s\n",(*fs->name)());
    list_add(fs_list, fs);
}

struct fs_filesystem* fs_find(uint8_t* name) {
    ASSERT_NOT_NULL(name, "name cannot be null");
    for (uint8_t i=0; i< list_count(fs_list);i++){
        struct fs_filesystem* fs = (struct fs_filesystem*) list_get(fs_list,i);
        if (strcmp(name, (*fs->name)())==0){
            return fs;
        }
    }
    return 0;
}

/*
* make a new directory listing
*/
struct fs_directory_listing* fs_directory_listing_new() {
    struct fs_directory_listing* ret = (struct fs_directory_listing*) kmalloc(sizeof(struct fs_directory_listing));
    ret->lst = list_new();
    return ret;
}

/*
* ugh
*/
void fs_directory_listing_delete(struct fs_directory_listing* listing) {
    ASSERT_NOT_NULL(listing, "listing cannot be null");
    ASSERT_NOT_NULL(listing->lst, "lst cannot be null");

    for (uint32_t i; i< list_count(listing->lst);i++){
        struct fs_directory* dir = (struct fs_directory *) list_get(listing->lst, i);
        if (0!=dir){
            kfree(dir);
        }
    }
    kfree(listing);
}
