//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DFS_MAP_H
#define _DFS_MAP_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>

#define DFS_MAP_BLOCK_FREE 0
#define DFS_MAP_BLOCK_USED 1

uint32_t dfs_map_find_free_block(struct device* dev);
void dfs_map_release_block(struct device* dev, uint64_t block);
uint32_t dfs_map_block_count(struct device* dev);
#endif