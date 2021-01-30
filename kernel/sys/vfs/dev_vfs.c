//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <sys/vfs/dev_vfs.h>

void dev_vfs_open(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}
void dev_vfs_read(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}
void dev_vfs_write(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}
void dev_vfs_close(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

struct vfs* vfs_new_dev(uint8_t* devicename) {
    ASSERT_NOT_NULL(devicename);
    struct vfs* ret = (struct vfs*)kmalloc(sizeof(struct vfs));
    ret->children = 0;
    ret->type = device;
    ret->close = &dev_vfs_close;
    ret->open = &dev_vfs_open;
    ret->read = &dev_vfs_read;
    ret->write = &dev_vfs_write;
    vfs_set_name(ret, devicename);
    return ret;
}
