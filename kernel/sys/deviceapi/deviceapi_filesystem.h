//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEAPI_FILESYSTEM_H
#define _DEVICEAPI_FILESYSTEM_H

#include <sys/collection/arraylist/arraylist.h>
#include <sys/devicemgr/devicemgr.h>
#include <types.h>

/*
 * format a file system
 */
typedef void (*fs_format_function)(struct device* dev);

/*
 * read file
 */
typedef void (*fs_read_function)(struct device* dev, const uint8_t* name, const uint8_t* data, uint32_t size);
/*
 * write file
 */
typedef void (*fs_write_function)(struct device* dev, const uint8_t* name, const uint8_t* data, uint32_t size);

struct deviceapi_filesystem {
    fs_format_function format;
    fs_read_function read;
    fs_write_function write;
};

#endif