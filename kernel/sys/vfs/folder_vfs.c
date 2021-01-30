//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <sys/vfs/dev_vfs.h>

void folder_vfs_open(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

void folder_vfs_read(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

void folder_vfs_write(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

void folder_vfs_close(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

void folder_vfs_readdir(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

struct vfs* vfs_new_folder(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    struct vfs* ret = (struct vfs*)kmalloc(sizeof(struct vfs));
    ret->type = folder;
    ret->close = &folder_vfs_close;
    ret->open = &folder_vfs_open;
    ret->read = &folder_vfs_read;
    ret->write = &folder_vfs_write;
    ret->readdir = &folder_vfs_readdir;
    vfs_set_name(ret, name);
    return ret;
}
