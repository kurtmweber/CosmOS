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
#include <sys/fs/fs.h>

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

enum fat_type {
	FAT12 = 0x01,
	FAT16 = 0x02,
	FAT32 = 0x03,
	ExFAT = 0x04
};

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
//	debug_show_memblock(buffer, sector_size);

    struct fat_BS* fat_boot = (struct fat_BS*) buffer;
    struct fat_extBS_16* fat_boot_ext_16 = (struct fat_extBS_16*) &(fat_boot->extended_section);
    struct fat_extBS_32* fat_boot_ext_32 = (struct fat_extBS_32*) &(fat_boot->extended_section);
//    fat_dump_fat_extBS_16(ebs);

    uint32_t total_sectors = (fat_boot->total_sectors_16 == 0)? fat_boot->total_sectors_32 : fat_boot->total_sectors_16;
    uint32_t fat_size = (fat_boot->table_size_16 == 0)? fat_boot_ext_32->table_size_32 : fat_boot->table_size_16;
    uint32_t root_dir_sectors = ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
    uint32_t first_data_sector = fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors;
    uint32_t first_fat_sector = fat_boot->reserved_sector_count;
    uint32_t data_sectors = fat_boot->total_sectors_16 - (fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors);
    uint32_t total_clusters = data_sectors / fat_boot->sectors_per_cluster;

    kprintf("total_clusters: %llu\n", total_clusters);
}

enum fat_type fat_fat_type(uint32_t total_clusters){
    if(total_clusters < 4085) {
        return  FAT12;
    } else if(total_clusters < 65525){
        return FAT16;
    } else if (total_clusters < 268435445) {
        return FAT32;
    } else { 
        return ExFAT;
    }
}

const uint8_t FAT_NAME[] = {"fat"};

const uint8_t* fat_name() {
    return FAT_NAME;
}

void fat_format(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");    
}

void fat_register() {
    struct filesystem* fs = (struct filesystem*) kmalloc(sizeof(struct filesystem));
    fs->format = &fat_format;
    fs->list_dir= &fat_list_dir;
    fs->name = &fat_name;
    fs_register(fs);
}