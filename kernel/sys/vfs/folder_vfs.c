//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <sys/vfs/dev_vfs.h>

void folder_vfs_open(struct vfs* v, uint8_t read, uint8_t write) {
    ASSERT_NOT_NULL(v);
}

uint32_t folder_vfs_read(struct vfs* v, uint32_t offset, uint32_t size, uint8_t* buffer) {
    ASSERT_NOT_NULL(v);
    return 0;
}

uint32_t folder_vfs_write(struct vfs* v, uint32_t offset, uint32_t size, uint8_t* buffer) {
    ASSERT_NOT_NULL(v);
    return 0;
}

void folder_vfs_close(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

void folder_vfs_readdir(struct vfs* v, uint32_t index) {
    ASSERT_NOT_NULL(v);
}

struct vfs* vfs_new_folder(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    struct vfs* ret = (struct vfs*)kmalloc(sizeof(struct vfs));
    memset((uint8_t*)ret, 0, sizeof(struct vfs));
    ret->type = folder;
    ret->close = &folder_vfs_close;
    ret->open = &folder_vfs_open;
    ret->read = &folder_vfs_read;
    ret->write = &folder_vfs_write;
    ret->readdir = &folder_vfs_readdir;
    vfs_set_name(ret, name);
    return ret;
}
