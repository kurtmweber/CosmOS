//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* vfs nodes for devices. 
*/
#ifndef _DEVVFS_H
#define _DEVVFS_H

#include <sys/vfs/vfs.h>

struct vfs* vfs_new_dev(uint8_t* devicename);

#endif