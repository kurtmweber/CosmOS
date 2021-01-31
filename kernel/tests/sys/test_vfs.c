//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <sys/vfs/folder_vfs.h>
#include <sys/vfs/vfs.h>

void test_vfs() {
    kprintf("Testing vfs\n");

    struct vfs* v = vfs_new_folder((uint8_t*)"/");
    ASSERT(v == vfs_find(v, "/"));
    ASSERT(0 == strcmp(v->name, "/"));

    v = vfs_find(cosmos_vfs, "/");
    ASSERT_NOT_NULL(v);
    ASSERT(0 == strcmp(v->name, "/"));

    v = vfs_find(cosmos_vfs, "/dev");
    ASSERT_NOT_NULL(v);
    ASSERT(0 == strcmp(v->name, "dev"));
}