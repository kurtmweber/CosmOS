//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VFS_H
#define _VFS_H

#include <sys/collection/arraylist/arraylist.h>
#include <types.h>

#define VFS_DEV_TREE "/localhost/dev"
#define VFS_FS_TREE "/localhost/fs"

enum vfs_type { file = 0x01, folder = 0x02, device = 0x03 };

struct vfs;

typedef void (*vfs_traverse_function)(struct vfs* v, uint32_t depth);
typedef void (*vfs_open_function)(struct vfs* v, uint8_t read, uint8_t write);
typedef uint32_t (*vfs_read_function)(struct vfs* v, uint32_t offset, uint32_t size, uint8_t* buffer);
typedef uint32_t (*vfs_write_function)(struct vfs* v, uint32_t offset, uint32_t size, uint8_t* buffer);
typedef void (*vfs_close_function)(struct vfs* v);
typedef void (*vfs_close_readdir_function)(struct vfs* v, uint32_t index);

struct vfs {
    enum vfs_type type;
    struct arraylist* children;

    /*
    This is a filename or a folder name
    For a device, or a fs, it's the dev name, since a fs is a dev
    */
    uint8_t* name;

    vfs_open_function open;
    vfs_read_function read;
    vfs_write_function write;
    vfs_close_function close;
    vfs_close_readdir_function readdir;
};

void vfs_init();

void vfs_delete(struct vfs* v);
void vfs_set_name(struct vfs* v, uint8_t* name);
void vfs_add_child(struct vfs* v, struct vfs* child);
void vfs_remove_child(struct vfs* v, uint8_t* name);
uint32_t vfs_count(struct vfs* v);
struct vfs* vfs_find(struct vfs* v, uint8_t* name);
void vfs_traverse(struct vfs* v, vfs_traverse_function f);
void vfs_dump(struct vfs* v);
extern struct vfs* cosmos_vfs;

#endif