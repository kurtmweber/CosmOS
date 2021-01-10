//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/dfs/dfs_block.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/fs/block_util.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/debug.h>
#include <sys/string/mem.h>
#include <sys/fs/fs.h>

/*
* read the superblock at lba 0
*/
void dfs_read_superblock(struct device* dev, struct dfs_superblock_block* superblock){
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(superblock, "superblock cannot be null");    
    block_read(dev, 0, (uint8_t*) superblock,1);
}

/*
* write the superblock at lba 0
*/
void dfs_write_superblock(struct device* dev, struct dfs_superblock_block* superblock){
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(superblock, "superblock cannot be null");    
    block_write(dev, 0, (uint8_t*) superblock,1);
}

/*
* write a dir block
*/
void dfs_write_dir_block(struct device* dev, struct dfs_dir_block* dir_block, uint64_t lba){
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(dir_block, "dir_block cannot be null");    
    block_write(dev, lba, (uint8_t*) dir_block,1);
}

/*
* read a dir block
*/
void dfs_read_dir_block(struct device* dev, struct dfs_dir_block* dir_block, uint64_t lba){
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(dir_block, "dir_block cannot be null");    
    block_read(dev, lba, (uint8_t*) dir_block,1);
}

