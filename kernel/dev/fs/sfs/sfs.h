//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SFS_H
#define _SFS_H

#include <types.h>

struct device* sfs_attach(struct device* block_device);
void sfs_detach(struct device* dev);

#endif