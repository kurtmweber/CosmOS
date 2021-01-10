//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/dfs/dfs_map.h>
#include <sys/fs/block_util.h>
#include <sys/fs/dfs/dfs_block.h>
#include <sys/debug/assert.h>
#include <sys/console/console.h>

/*
* find the block for the map entry for a block (indexed to first map block)
*/
uint64_t dfs_map_sector(uint32_t block) {
    return block/DFS_SECTORS_PER_MAP_BLOCK;
}

/*
* find the byte index for the map entry for a block (indexed to first map block)
*/
uint64_t dfs_map_index(uint32_t block) {
    return block-(dfs_map_sector(block)*DFS_SECTORS_PER_MAP_BLOCK);
}

/*
* returns zero if no block can be found (disk is full)
*/
// TODO this indexes more blocks than are on the disk :(
uint32_t dfs_map_find_free_block(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    for (uint64_t block=1; block <= dfs_map_block_count(dev);block++){
        struct dfs_map_block mapblock;
        dfs_read_map_block(dev, &mapblock, block);
        for (int i=0; i<DFS_BLOCK_SIZE;i++){
            if (mapblock.map[i]==DFS_MAP_BLOCK_FREE){
                mapblock.map[i]=DFS_MAP_BLOCK_USED;
                return i; 
            }
        }
    }
    return 0;
}

void dfs_map_release_block(struct device* dev, uint64_t block) {
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    uint64_t sector = dfs_map_sector(block);
    uint64_t index = dfs_map_index(block);
    kprintf("block %llu, sector %llu, index %llu\n", block, sector, index);
}

uint32_t dfs_map_block_count(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null"); 
    return (block_get_sector_count(dev) / DFS_SECTORS_PER_MAP_BLOCK)+1;
}



