//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FAT_H
#define _FAT_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/collection/list/list.h>

struct fat_boot_block_parameters {
    uint8_t jmp[3]; // 0xeb 0x3c 0x90
    uint8_t oem[8];
    uint8_t bytes_per_sector[2];
    uint8_t clusters_per_sector;
    uint8_t reserved_sectors[2];
    uint8_t number_fat;
    uint8_t number_directory_enties[2];
    uint8_t total_sectors[2];
    uint8_t media_descriptor_type;
    uint8_t sectors_per_fat[2];
    uint8_t sectors_per_track[2];
    uint8_t heads[2];
    uint8_t number_hidden_sectors[4];
    uint8_t large_sector_count[4];
} __attribute__((packed));


struct fat_extended_boot_record {
    uint8_t drive_number;
    uint8_t flags;
    uint8_t reserved; // 0x28 or 0x29
    uint8_t volume_id[4];
    uint8_t volume_label[11];
    uint8_t sys_id[8];
    uint8_t boot_code[448];
    uint8_t signature[2]; // 0xaa 0x55

} __attribute__((packed));

void fat_list_dir(struct device* dev, struct list* lst);

#endif