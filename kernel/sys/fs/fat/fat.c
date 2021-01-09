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

// Extended fat32 stuff=
struct fat_extBS_32 {
	uint32_t	table_size_32;
	uint16_t	extended_flags;
	uint16_t	fat_version;
	uint32_t	root_cluster;
	uint16_t	fat_info;
	uint16_t	backup_BS_sector;
	uint8_t 	reserved_0[12];
	uint8_t		drive_number;
	uint8_t 	reserved_1;
	uint8_t		boot_signature;
	uint32_t 	volume_id;
	uint8_t		volume_label[11];
	uint8_t		fat_type_label[8];
}__attribute__((packed));
 
// Extended fat12 and fat16 stuff
struct fat_extBS_16 {
	uint8_t		bios_drive_num;
	uint8_t		reserved1;
	uint8_t		boot_signature;
	uint32_t	volume_id;
	uint8_t		volume_label[11];
	uint8_t		fat_type_label[8];
}__attribute__((packed));
 
struct fat_BS {
	uint8_t 	bootjmp[3];
	uint8_t 	oem_name[8];
	uint16_t 	bytes_per_sector;
	uint8_t		sectors_per_cluster;
	uint16_t	reserved_sector_count;
	uint8_t		table_count;
	uint16_t	root_entry_count;
	uint16_t	total_sectors_16;
	uint8_t		media_type;
	uint16_t	table_size_16;
	uint16_t	sectors_per_track;
	uint16_t	head_side_count;
	uint32_t 	hidden_sector_count;
	uint32_t 	total_sectors_32;
 
	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	uint8_t		extended_section[54];
}__attribute__((packed));

void fat_dump_fat_extBS_16(struct fat_extBS_16* ebs) {
    kprintf("drive_number %llu\n", ebs->bios_drive_num);
  //  kprintf("volume_id %llu\n", ebs->volume_id);
    kprintf("volume_label %s\n", ebs->volume_label);
  //  kprintf("fat_type_label %s \n", ebs->fat_type_label);

 //   debug_show_memblock((uint8_t*)ebs, sizeof(struct fat_extBS_16));
}

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

    struct fat_BS* bs = (struct fat_BS*) buffer;
    struct fat_extBS_16* ebs = (struct fat_extBS_16*) &(bs->extended_section);
//    fat_dump_fat_extBS_16(ebs);
}


