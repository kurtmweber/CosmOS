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
#include <sys/console/console.h>

/*
* returns file block, or zero
*/
uint64_t dfs_dir_find_file(struct device* dev, uint8_t* filename) {
    ASSERT_NOT_NULL(dev); 
    ASSERT_NOT_NULL(filename); 
    ASSERT(strlen(filename)<DFS_FILENAME_SIZE);
}

/*
* returns file block, or zero
*/
uint64_t dfs_dir_add_file(struct device* dev, uint8_t* filename) {
    ASSERT_NOT_NULL(dev); 
    ASSERT(strlen(filename)<DFS_FILENAME_SIZE);
    ASSERT_NOT_NULL(filename); 

}

void dfs_dir_iterate_files(struct device* dev, dfs_file_iterator file_iterator) {
    ASSERT_NOT_NULL(file_iterator); 
    ASSERT_NOT_NULL(dev); 
    /*
    * get the superblock
    */
    struct dfs_superblock_block superblock;
    dfs_read_superblock(dev, &superblock);
    ASSERT(superblock.magic==0x00444653);
    kprintf("Superblock blocks_size %llu blocks_count %llu\n", superblock.blocks_size, superblock.blocks_count);
    /*
    * iterate directory blocks
    */
    bool more=true;
    uint64_t block=superblock.root_dir;
    while (block) {
        /*
        * get the root dir
        */
        struct dfs_dir_block root_dir;
        dfs_read_dir_block(dev, &root_dir, block);
        for (uint32_t i=0; i<DFS_FILES_PER_DIR_BLOCK;i++) {
            uint64_t file_block_block = root_dir.files[i];
            /*
            * get the file for that one
            */
            struct dfs_file_block file_block;
            dfs_read_file_block(dev, &file_block, file_block_block);
            /*
            * callback
            */ 
            bool cont = file_iterator(&file_block);
            if (cont==false){
                break;
            }
        }
        if (root_dir.next!=0){
            block = root_dir.next;
        } else {
            block = 0;
        }
    }
}




