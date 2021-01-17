//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TFS_MAP_H
#define _TFS_MAP_H

#include <dev/fs/tfs/tfs_block.h>
#include <sys/devicemgr/devicemgr.h>
#include <types.h>

#define DFS_MAP_BLOCK_FREE 0
#define DFS_MAP_BLOCK_USED 1

uint32_t tfs_map_find_free_block(struct device* dev, struct tfs_superblock_block* superblock);
void tfs_map_release_block(struct device* dev, uint64_t block, struct tfs_superblock_block* superblock);
uint32_t tfs_map_block_count(struct device* dev);
#endif