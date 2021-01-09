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

typedef void (*fs_format)(struct device* dev);
typedef void (*fs_list_dir)(struct device* dev, struct list* lst);
typedef const uint8_t* (*fs_name)();

struct filesystem {
    fs_format format;
    fs_list_dir list_dir;
    fs_name name;
};

/*
* register all fs's
*/
void fs_init();

/*
* register a fs
*/
void fs_register(struct filesystem* fs);

/*
* get an fs by name
*/
struct filesystem* fs_find(uint8_t* name);

#endif