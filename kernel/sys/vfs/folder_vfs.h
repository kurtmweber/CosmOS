//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* vfs nodes for folder 
*/
#ifndef _FOLDERVFS_H
#define _FOLDERVFS_H

#include <sys/vfs/vfs.h>

struct vfs* vfs_new_folder(uint8_t* devicename);

#endif