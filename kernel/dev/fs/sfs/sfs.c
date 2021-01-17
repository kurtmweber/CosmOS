//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/fs.h>
#include <dev/fs/sfs/sfs.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

#define SFS_VOLUME_IDENTIFIER 0x01
#define SFS_STARTING_MARKER 0x02
#define SFS_UNUSED_ENTRY 0x10
#define SFS_DIRECTORY_ENTRY 0x11
#define SFS_FILE_ENTRY 0x12
#define SFS_UNUSABLE_ENTRY 0x18
#define SFS_DELETED_DIRECTORY_ENTRY 0x19
#define SFS_DELETED_FILE_ENTRY 0x1A
#define SFS_CONTINUATION_ENTRY 0x20

struct sfs_superblock {
    uint8_t reserved1[11];
    uint8_t reserved2[21];
    uint8_t reserved3[372];
    uint64_t timestamp;
    uint64_t dataarea_size_blocks;
    uint64_t indexarea_size_bytes;
    uint8_t magic[3];
    uint8_t version;
    uint64_t total_blocks;
    uint32_t reserved_blocks;
    uint8_t block_size;
    uint8_t checksum;
    uint8_t reserved4[64];
    uint8_t reserved5[2];
} __attribute__((packed));

struct sfs_volume_identifier {
    uint8_t entry_type;
    uint8_t reserved[3];
    uint64_t timestamp;
    uint8_t name[52];
} __attribute__((packed));

struct sfs_starting_marker {
    uint8_t entry_type;
    uint8_t reserved[63];
} __attribute__((packed));

struct sfs_unused_entry {
    uint8_t entry_type;
    uint8_t reserved[63];
} __attribute__((packed));

struct sfs_directory_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint8_t reserved[54];
} __attribute__((packed));

struct sfs_file_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint64_t start_block;
    uint64_t end_block;
    uint64_t file_length;
    uint8_t name[30];
} __attribute__((packed));

struct sfs_unusable_entry {
    uint8_t entry_type;
    uint8_t reserved1[9];
    uint64_t start_block;
    uint64_t end_block;
    uint8_t reserved2[38];
} __attribute__((packed));

struct sfs_deleted_directory_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint8_t name[54];
} __attribute__((packed));

struct sfs_deleted_file_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint64_t start_block;
    uint64_t end_block;
    uint64_t file_length;
    uint8_t name[30];
} __attribute__((packed));

struct sfs_continuation_entry {
    uint8_t name[64];
} __attribute__((packed));

/*
 * check if valid superblock
 */
bool sfs_is_valid_superblock(struct sfs_superblock* superblock) {
    ASSERT_NOT_NULL(superblock);
    if ((superblock->magic[0] == 0x53) && (superblock->magic[1] == 0x46) && (superblock->magic[2] == 0x53)) {
        return true;
    }
    return false;
}

void sfs_read_superblock(struct device* dev, struct sfs_superblock* superblock) {
    ASSERT_NOT_NULL(dev);
    block_read(dev, 0, (uint8_t*)superblock, 1);
}

void sfs_format(struct device* dev) {
    ASSERT_NOT_NULL(dev);

    // device parameters
    uint64_t total_size = block_get_total_size(dev);
    uint32_t sector_size = block_get_sector_size(dev);
    uint32_t total_sectors = block_get_sector_count(dev);

    // create a superblock struct
    struct sfs_superblock superblock;
    memset((uint8_t*)&superblock, 0, sizeof(struct sfs_superblock));
    superblock.timestamp = 0;                             // later
    superblock.dataarea_size_blocks = total_sectors - 2;  // 1 for superblock and 1 for index
    superblock.indexarea_size_bytes = sector_size;        // 1 sector
    superblock.reserved_blocks = 1;                       // 1, for the superblock
    superblock.total_blocks = total_sectors;
    superblock.version = 0x10;  // 1.0
    superblock.magic[0] = 0x53;
    superblock.magic[1] = 0x46;
    superblock.magic[2] = 0x53;
    superblock.block_size = (sector_size / 512) + 1;

    // write superblock
    block_write(dev, 0, (uint8_t*)&superblock, 1);
}

struct fs_directory_listing* sfs_list_dir(struct device* dev) {
    ASSERT_NOT_NULL(dev);
}

const uint8_t SFS_NAME[] = {"sfs"};

const uint8_t* sfs_name() {
    return SFS_NAME;
}

void sfs_register() {
    struct fs_filesystem* fs = (struct fs_filesystem*)kmalloc(sizeof(struct fs_filesystem));
    fs->format = &sfs_format;
    fs->list_dir = &sfs_list_dir;
    fs->name = &sfs_name;
    fs_register(fs);
}