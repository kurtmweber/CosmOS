//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _MBR_PT_H
#define _MBR_PT_H

#include <types.h>

struct mbr_pt_entry {
    uint8_t attributes;
    uint8_t chs_start[3];
    uint8_t partition_type;
    uint8_t chs_last[3];
    uint32_t lba_start;
    uint32_t total_sectors;
} __attribute__((packed));

struct mbr_pt_header {
    uint8_t boot[440];
    uint32_t disk_id;
    uint8_t reserved[2];
    struct mbr_pt_entry partitions[4];
    uint8_t signature[2];
} __attribute__((packed));

struct device;

struct device* mbr_pt_attach(struct device* block_device);
void mbr_pt_detach(struct device* dev);

#endif