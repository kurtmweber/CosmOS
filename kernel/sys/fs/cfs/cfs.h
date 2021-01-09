//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CFS_H
#define _CFS_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/collection/list/list.h>

void cfs_list_dir(struct device* dev, struct list* lst);
void cfs_format(struct device* dev);
#endif