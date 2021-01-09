//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SFS_H
#define _SFS_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>

void sfs_format(struct device* dev);

#endif