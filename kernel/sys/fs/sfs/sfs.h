//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SFS_H
#define _SFS_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>

#define SFS_VOLUME_IDENTIFIER           0x01
#define SFS_STARTING_MARKER             0x02
#define SFS_UNUSED_ENTRY                0x10
#define SFS_DIRECTORY_ENTRY             0x11
#define SFS_FILE_ENTRY                  0x12
#define SFS_UNUSABLE_ENTRY              0x18
#define SFS_DELETED_DIRECTORY_ENTRY     0x19
#define SFS_DELETED_FILE_ENTRY          0x1A
#define SFS_CONTINUATION_ENTRY          0x20

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

void sfs_format(struct device* dev);

#endif