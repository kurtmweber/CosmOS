//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DFS_BLOCK_H
#define _DFS_BLOCK_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

#define DFS_BLOCK_SIZE 512
#define DFS_FILES_PER_DIR_BLOCK 63
#define DFS_FILES_PER_MAP_BLOCK 63
#define DFS_FILENAME_SIZE 128
#define DFS_SECTORS_PER_MAP_BLOCK 512
#define DFS_MAGIC_SUPERBLOCK 0x00444653  // 'DFS'

// 512 bytes which are 64 uint64_t's
struct tfs_superblock_block {
    uint64_t magic;              // DFS_MAGIC_SUPERBLOCK
    uint64_t blocks_size;        // bytes per block
    uint64_t blocks_count;       // total blocks on disk including superblock, etc
    uint64_t number_map_blocks;  // total number map blocks.  these follow the superblock, in order.
    uint64_t root_dir;           // block of root dir
    uint64_t reserved[59];
} __attribute__((packed));

// the number of files determined by DFS_BLOCK_SIZE
struct tfs_dir_block {
    uint64_t files[DFS_FILES_PER_DIR_BLOCK];  // pointers to file blocks
    uint64_t next;                            // next dir block, or zero if no more
} __attribute__((packed));

struct tfs_file_block {
    uint8_t name[DFS_FILENAME_SIZE];  // file name
    uint64_t allocation_map;          // allocation map block
    uint64_t status;                  // deleted, etc.
    uint8_t reserved[368];
} __attribute__((packed));

// the number of blocks we can reference here is determined by DFS_BLOCK_SIZE
struct tfs_file_allocation_block {
    uint64_t blocks[DFS_FILES_PER_MAP_BLOCK];  // pointers to data blocks
    uint64_t allocation_map;                   // next dir block, or zero if no more
} __attribute__((packed));

struct tfs_map_block {
    uint8_t map[DFS_SECTORS_PER_MAP_BLOCK];
} __attribute__((packed));

void tfs_read_superblock(struct device* dev, struct tfs_superblock_block* superblock);
void tfs_write_superblock(struct device* dev, struct tfs_superblock_block* superblock);
void tfs_write_dir_block(struct device* dev, struct tfs_dir_block* dir_block, uint64_t lba);
void tfs_read_dir_block(struct device* dev, struct tfs_dir_block* dir_block, uint64_t lba);
void tfs_write_map_block(struct device* dev, struct tfs_map_block* map_block, uint64_t lba);
void tfs_read_map_block(struct device* dev, struct tfs_map_block* map_block, uint64_t lba);
void tfs_write_file_block(struct device* dev, struct tfs_file_block* file_block, uint64_t lba);
void tfs_read_file_block(struct device* dev, struct tfs_file_block* file_block, uint64_t lba);

#endif