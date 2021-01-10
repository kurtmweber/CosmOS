//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/dfs/dfs.h>
#include <sys/fs/dfs/dfs_block.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/fs/block_util.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/debug.h>
#include <sys/string/mem.h>
#include <sys/fs/fs.h>

const uint8_t DFS_NAME[] = {"dfs"};

const uint8_t* dfs_name() {
    return DFS_NAME;
}

/*
* format. I just guessed here. 
*/
void dfs_format(struct device* dev) {
    /*
    * create superblock
    */
    struct dfs_superblock_block superblock;
    memset((uint8_t*)&superblock,0,sizeof(struct dfs_superblock_block));
    superblock.magic = DFS_MAGIC_SUPERBLOCK;
    superblock.blocks_size = (uint64_t) block_get_sector_size(dev);
    superblock.blocks_count = (uint64_t) block_get_sector_count(dev);
    superblock.root_dir=1; // sector one, since sector zero is the super-block
    /*
    * write superblock
    */
    dfs_write_superblock(dev,&superblock);
    /*
    * root dir block
    */
    struct dfs_dir_block root_dir_block;
    memset((uint8_t*)&root_dir_block,0,sizeof(struct dfs_dir_block));
    root_dir_block.next=0;
    /*
    * write root dir
    */
    dfs_write_dir_block(dev,&root_dir_block, superblock.blocks_count);
}

struct fs_directory_listing* dfs_list_dir(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    /*
    * get the superblock
    */
    struct dfs_superblock_block superblock;
    dfs_read_superblock(dev, &superblock);
    ASSERT(superblock.magic==0x00444653,"Invalid superblock");
    kprintf("Superblock blocks_size %llu blocks_count %llu\n", superblock.blocks_size, superblock.blocks_count);
    /*
    * iterate directory blocks
    */
    bool more=true;
    uint64_t block=superblock.root_dir;
    while (more) {
        /*
        * get the root dir
        */
        struct dfs_dir_block root_dir;
        dfs_read_dir_block(dev, &root_dir, superblock.root_dir);
        for (uint32_t i=0; i<DFS_FILES_PER_DIR_BLOCK;i++) {
            uint64_t file_block =  root_dir.files[i];
            if (0!=file_block){
                kprintf("dfs file at block %llu\n",i);
            }  
        }        
    }
}

void dfs_register() {
    ASSERT(sizeof(struct dfs_superblock_block)==DFS_BLOCK_SIZE, "dfs_superblock_block must be 512 bytes");
    ASSERT(sizeof(struct dfs_dir_block)==DFS_BLOCK_SIZE, "dfs_dir_block must be 512 bytes");
    ASSERT(sizeof(struct dfs_file_block)==DFS_BLOCK_SIZE, "dfs_file_block must be 512 bytes");
    ASSERT(sizeof(struct dfs_filemap_block)==DFS_BLOCK_SIZE, "dfs_filemap_block must be 512 bytes");

    struct fs_filesystem* fs = (struct fs_filesystem*) kmalloc(sizeof(struct fs_filesystem));
    fs->format = &dfs_format;
    fs->list_dir= &dfs_list_dir;
    fs->name = &dfs_name;
    fs_register(fs);
}

