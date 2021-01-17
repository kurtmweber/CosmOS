//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/tfs/tfs_block.h>
#include <dev/fs/tfs/tfs_map.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>

/*
 * find the block for the map entry for a block (indexed to first map block)
 */
uint64_t tfs_map_sector(uint32_t block) {
    return block / DFS_SECTORS_PER_MAP_BLOCK;
}

/*
 * find the byte index for the map entry for a block (indexed to first map block)
 */
uint64_t tfs_map_index(uint32_t block) {
    return block - (tfs_map_sector(block) * DFS_SECTORS_PER_MAP_BLOCK);
}

/*
 * returns zero if no block can be found (disk is full)
 */
uint32_t tfs_map_find_free_block(struct device* dev, struct tfs_superblock_block* superblock) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(superblock);
    uint64_t byte_count = 0;
    for (uint64_t block = 1; block <= tfs_map_block_count(dev); block++) {
        struct tfs_map_block mapblock;
        tfs_read_map_block(dev, &mapblock, block);
        for (uint64_t i = 0; i < DFS_BLOCK_SIZE; i++) {
            if (mapblock.map[i] == DFS_MAP_BLOCK_FREE) {
                mapblock.map[i] = DFS_MAP_BLOCK_USED;
                return i;
            }
            /*
             * make sure we dont index off the end
             */
            byte_count++;
            if (byte_count >= superblock->blocks_count) {
                // ran out of blocks
                return 0;
            }
        }
    }
    return 0;
}

void tfs_map_release_block(struct device* dev, uint64_t block, struct tfs_superblock_block* superblock) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(superblock);
    uint64_t sector = tfs_map_sector(block);
    uint64_t index = tfs_map_index(block);
    kprintf("block %llu, sector %llu, index %llu\n", block, sector, index);
}

uint32_t tfs_map_block_count(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    return (block_get_sector_count(dev) / DFS_SECTORS_PER_MAP_BLOCK) + 1;
}
