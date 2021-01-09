//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/fat/fat.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/fs/block_util.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/debug.h>
#include <sys/string/mem.h>

// https://wiki.osdev.org/FAT

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

void fat_list_dir(struct device* dev, struct list* lst) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(lst, "lst cannot be null");

    uint16_t sector_size = block_get_sector_size(dev);
    kprintf("sector size: %llu\n", sector_size);

    uint32_t total_size = block_get_total_size(dev);
    kprintf("total size: %llu\n", total_size);

    uint8_t* buffer = kmalloc(sector_size);
	memset(buffer, 0, sector_size);

    block_read(dev, 0, buffer,1);
	debug_show_memblock(buffer, sector_size);

    struct fat_boot_block_parameters* fbbp = (struct fat_boot_block_parameters*) buffer;
    kprintf("OEM %s\n", fbbp->oem);
}

