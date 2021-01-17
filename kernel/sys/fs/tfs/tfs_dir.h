//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TFS_DIR_H
#define _TFS_DIR_H

#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/tfs/tfs_block.h>
#include <types.h>

typedef bool (*tfs_file_iterator)(struct tfs_file_block* file_block);

/*
 * find file. returns file block, or zero
 */
uint64_t tfs_dir_find_file(struct device* dev, uint8_t* filename);
/*
 * add file. returns file block, or zero
 */
uint64_t tfs_dir_add_file(struct device* dev, uint8_t* filename);
/*
 * remove file.
 */
void tfs_dir_remove_file(struct device* dev, uint8_t* filename);
/*
 * iterate files
 */
void tfs_dir_iterate_files(struct device* dev, tfs_file_iterator file_iterator);

#endif