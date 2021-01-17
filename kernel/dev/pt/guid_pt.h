//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _GUID_PT_H
#define _GUID_PT_H

#include <types.h>

struct device;

struct guid_pt_header {
    uint8_t magic[64];
    uint32_t gpt_revision;
    uint32_t header_size;
    uint32_t crc32;
    uint32_t reserved;
    uint64_t gpt_lba;
    uint64_t alt_gpt_lba;
    uint64_t first_block;
    uint64_t last_block;
    uint64_t disk_guid[2];
    uint64_t gpt_array_lba;
    uint32_t num_partitions;
    uint32_t entry_size;
    uint32_t array_crc32;
} __attribute__((packed));

struct guid_pt_entry {
    uint8_t partition_type[16];
    uint8_t partition_id[16];
    uint64_t start_lba;
    uint64_t end_lba;
    uint64_t attributes;
    uint8_t name[72];
} __attribute__((packed));

struct device;

struct device* guid_pt_attach(struct device* block_device);
void guid_pt_detach(struct device* dev);

#endif