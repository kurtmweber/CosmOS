//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/dfs/dfs.h>
#include <sys/fs/dfs/dfs_block.h>
#include <sys/fs/dfs/dfs_map.h>
#include <sys/fs/dfs/dfs_dir.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/fs/block_util.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/debug.h>
#include <sys/string/mem.h>
#include <sys/fs/fs.h>
#include <sys/string/string.h>

const uint8_t DFS_NAME[] = {"dfs"};

const uint8_t* dfs_name() {
    return DFS_NAME;
}

/*
* format. I just guessed here. 
*/
void dfs_format(struct device* dev) {
    /*
    * figure out how many map blocks we need
    */
    uint32_t number_map_blocks = dfs_map_block_count(dev);
    /*
    * create superblock
    */
    struct dfs_superblock_block superblock;
    memset((uint8_t*)&superblock,0,sizeof(struct dfs_superblock_block));
    superblock.magic = DFS_MAGIC_SUPERBLOCK;
    superblock.blocks_size = (uint64_t) block_get_sector_size(dev);
    superblock.blocks_count = (uint64_t) block_get_sector_count(dev);
    superblock.number_map_blocks = number_map_blocks;
    superblock.root_dir=number_map_blocks+1; // sector one, since sector zero is the super-block
    kprintf("blocks_count %llu\n", superblock.blocks_count);
    kprintf("number_map_blocks %llu\n", superblock.number_map_blocks);
    kprintf("root_dir %llu\n", superblock.root_dir);
    /*
    * write superblock
    */
    dfs_write_superblock(dev,&superblock);
    /*
    * create & write map blocks
    */
    for (uint32_t i=0; i<number_map_blocks;i++){
        struct dfs_map_block map_block;
        memset((uint8_t*)&map_block,0,sizeof(struct dfs_map_block));
        dfs_write_map_block(dev, &map_block, i+1);
    //    kprintf("map block: %llu\n",i+1);
    }
    /*
    * root dir block
    */
    struct dfs_dir_block root_dir_block;
    memset((uint8_t*)&root_dir_block,0,sizeof(struct dfs_dir_block));
    root_dir_block.next=0;
    /*
    * write root dir
    */
    dfs_write_dir_block(dev,&root_dir_block, superblock.root_dir);
    kprintf("dir block: %llu\n",superblock.root_dir);
}

bool dfs_dir_list_iterator(struct dfs_file_block* file_block) {
    ASSERT_NOT_NULL(file_block, "file_block cannot be null"); 
    kprintf("dfs file at block %s\n",file_block->name);
    return true;
}

struct fs_directory_listing* dfs_list_dir(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");  
    dfs_dir_iterate_files(dev, &dfs_dir_list_iterator);
}

void dfs_read(struct device* dev, const uint8_t* name, const uint8_t* data, uint32_t size){
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    ASSERT_NOT_NULL(name, "name cannot be null"); 
    ASSERT_NOT_NULL(data, "data cannot be null"); 
    ASSERT(strlen(name)<DFS_FILENAME_SIZE,"filename too long");
}

void dfs_write(struct device* dev, const uint8_t* name, const uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    ASSERT_NOT_NULL(name, "name cannot be null"); 
    ASSERT_NOT_NULL(data, "data cannot be null"); 
    ASSERT(strlen(name)<DFS_FILENAME_SIZE,"filename too long");

    kprintf("write file: %s of length %llu\n", name, size);
    /*
    * find an empty spot in the root dir
    */

    /*
    * write to the dir
    */

    /*
    * make an allocation block
    */
}

void dfs_register() {
    ASSERT(sizeof(struct dfs_superblock_block)==DFS_BLOCK_SIZE, "dfs_superblock_block must be 512 bytes");
    ASSERT(sizeof(struct dfs_dir_block)==DFS_BLOCK_SIZE, "dfs_dir_block must be 512 bytes");
    ASSERT(sizeof(struct dfs_file_block)==DFS_BLOCK_SIZE, "dfs_file_block must be 512 bytes");
    ASSERT(sizeof(struct dfs_file_allocation_block)==DFS_BLOCK_SIZE, "dfs_allocation_block must be 512 bytes");
    ASSERT(sizeof(struct dfs_map_block)==DFS_BLOCK_SIZE, "dfs_map_block must be 512 bytes");

    struct fs_filesystem* fs = (struct fs_filesystem*) kmalloc(sizeof(struct fs_filesystem));
    fs->format = &dfs_format;
    fs->list_dir= &dfs_list_dir;
    fs->name = &dfs_name;
    fs->read = &dfs_read;
    fs->write = &dfs_write;
    fs_register(fs);
}