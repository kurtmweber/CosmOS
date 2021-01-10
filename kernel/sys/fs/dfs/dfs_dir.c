//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/dfs/dfs_dir.h>
#include <sys/fs/block_util.h>
#include <sys/fs/dfs/dfs_block.h>
#include <sys/debug/assert.h>
#include <sys/string/string.h>

/*
* returns file block, or zero
*/
uint64_t dfs_dir_find_file(struct device* dev, uint8_t* filename) {
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    ASSERT_NOT_NULL(filename, "filename cannot be null"); 
    ASSERT(strlen(filename)<DFS_FILENAME_SIZE,"filename too long");

}

/*
* returns file block, or zero
*/
uint64_t dfs_dir_add_file(struct device* dev, uint8_t* filename) {
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    ASSERT(strlen(filename)<DFS_FILENAME_SIZE,"filename too long");
    ASSERT_NOT_NULL(filename, "filename cannot be null"); 

}



