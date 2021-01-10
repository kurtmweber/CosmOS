//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FS_H
#define _FS_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/collection/list/list.h>

/*
* format a file system
*/
typedef void (*fs_format)(struct device* dev);
/*
* returns a list of fs_file structs. remember to delete these!
*/
typedef struct fs_directory_listing* (*fs_list_dir)(struct device* dev);
/*
* name of type (fat, sfs, cfs, etc)
*/
typedef const uint8_t* (*fs_name)();

struct fs_filesystem {
    fs_format format;
    fs_list_dir list_dir;
    fs_name name;
};

struct fs_directory_listing {
    struct list* lst;
};

struct fs_file {
    uint8_t name[255];
    uint64_t size;
    uint32_t flags;
};

/*
* register all fs's
*/
void fs_init();

/*
* register a fs
*/
void fs_register(struct fs_filesystem* fs);

/*
* get an fs by name
*/
struct fs_filesystem* fs_find(uint8_t* name);

/*
* new directory listing
*/
struct fs_directory_listing* fs_directory_listing_new();

/*
* new directory listing
*/
void fs_directory_listing_delete(struct fs_directory_listing* listing);

#endif