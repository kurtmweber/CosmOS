//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/fs/block_util.h>
#include <sys/fs/fat/fat.h>
#include <sys/fs/fs.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

// https://wiki.osdev.org/FAT

// Extended fat32 stuff=
struct fat_extBS_32 {
    uint32_t table_size_32;
    uint16_t extended_flags;
    uint16_t fat_version;
    uint32_t root_cluster;
    uint16_t fat_info;
    uint16_t backup_BS_sector;
    uint8_t reserved_0[12];
    uint8_t drive_number;
    uint8_t reserved_1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t fat_type_label[8];
} __attribute__((packed));

// Extended fat12 and fat16 stuff
struct fat_extBS_16 {
    uint8_t bios_drive_num;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t fat_type_label[8];
} __attribute__((packed));

struct fat_BS {
    uint8_t bootjmp[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t table_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t media_type;
    uint16_t table_size_16;
    uint16_t sectors_per_track;
    uint16_t head_side_count;
    uint32_t hidden_sector_count;
    uint32_t total_sectors_32;

    // this will be cast to it's specific type once the driver actually knows what type of FAT this is.
    uint8_t extended_section[54];
} __attribute__((packed));

struct fat_dir {
    uint8_t name[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creation_time;
    uint16_t time;
    uint16_t date;
    uint16_t last_accessed;
    uint16_t cluster_high;
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t cluster_low;
    uint32_t size;
} __attribute__((packed));

enum fat_type { FAT12 = 0x01, FAT16 = 0x02, FAT32 = 0x03, ExFAT = 0x04 };

/*
 * calculated parameters of this file system
 */
struct fat_fs_parameters {
    // from fs_BS
    uint8_t sectors_per_cluster;
    // from device
    uint16_t sector_size;
    // from device
    uint32_t total_size;
    /*
     * rest of these are calculated
     */
    uint32_t total_sectors;
    uint32_t fat_size;
    uint32_t root_dir_sectors;
    uint32_t first_data_sector;
    uint32_t first_fat_sector;
    uint32_t data_sectors;
    uint32_t total_clusters;
    uint32_t first_root_dir_sector;
    uint32_t root_cluster_32;
    enum fat_type type;
};

const uint8_t FAT_NAME[] = {"fat"};

void fat_dump_fat_fs_parameters(struct fat_fs_parameters* param) {
    kprintf("sector size: %llu\n", param->sector_size);
    kprintf("total size: %llu\n", param->total_size);
    kprintf("total_sectors %llu\n", param->total_sectors);
    kprintf("fat_size %llu\n", param->fat_size);
    kprintf("root_dir_sectors %llu\n", param->root_dir_sectors);
    kprintf("first_data_sector %llu\n", param->first_data_sector);
    kprintf("first_fat_sector %llu\n", param->first_fat_sector);
    kprintf("data_sectors %llu\n", param->data_sectors);
    kprintf("total_clusters %llu\n", param->total_clusters);
    kprintf("first_root_dir_sector %llu\n", param->first_root_dir_sector);
    kprintf("root_cluster_32 %llu\n", param->root_cluster_32);
    kprintf("type: %llu\n", param->type);
    kprintf("sectors_per_cluster: %llu\n", param->sectors_per_cluster);
}

void fat_dump_fat_extBS_16(struct fat_extBS_16* ebs) {
    kprintf("drive_number %llu\n", ebs->bios_drive_num);
    //  kprintf("volume_id %llu\n", ebs->volume_id);
    kprintf("volume_label %s\n", ebs->volume_label);
    //  kprintf("fat_type_label %s \n", ebs->fat_type_label);

    //   debug_show_memblock((uint8_t*)ebs, sizeof(struct fat_extBS_16));
}

void fat_read_fs_parameters(struct device* dev, struct fat_fs_parameters* param) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(param);

    param->sector_size = block_get_sector_size(dev);
    param->total_size = block_get_total_size(dev);

    uint8_t* buffer = kmalloc(param->sector_size);
    memset(buffer, 0, param->sector_size);

    block_read(dev, 0, buffer, 1);

    struct fat_BS* fat_boot = (struct fat_BS*)buffer;
    struct fat_extBS_16* fat_boot_ext_16 = (struct fat_extBS_16*)&(fat_boot->extended_section);
    struct fat_extBS_32* fat_boot_ext_32 = (struct fat_extBS_32*)&(fat_boot->extended_section);

    param->sectors_per_cluster = fat_boot->sectors_per_cluster;
    param->total_sectors = (fat_boot->total_sectors_16 == 0) ? fat_boot->total_sectors_32 : fat_boot->total_sectors_16;
    param->fat_size = (fat_boot->table_size_16 == 0) ? fat_boot_ext_32->table_size_32 : fat_boot->table_size_16;
    param->root_dir_sectors = ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
    param->first_data_sector = fat_boot->reserved_sector_count + (fat_boot->table_count * param->fat_size) + param->root_dir_sectors;
    param->first_fat_sector = fat_boot->reserved_sector_count;
    param->data_sectors = fat_boot->total_sectors_16 - (fat_boot->reserved_sector_count + (fat_boot->table_count * param->fat_size) + param->root_dir_sectors);
    param->total_clusters = param->data_sectors / fat_boot->sectors_per_cluster;
    param->first_root_dir_sector = param->first_data_sector - param->root_dir_sectors;
    param->root_cluster_32 = fat_boot_ext_32->root_cluster;

    // fat type
    if (param->total_clusters < 4085) {
        param->type = FAT12;
    } else if (param->total_clusters < 65525) {
        param->type = FAT16;
    } else if (param->total_clusters < 268435445) {
        param->type = FAT32;
    } else {
        param->type = ExFAT;
    }
    kfree(buffer);
}

const uint8_t* fat_name() {
    return FAT_NAME;
}

void fat_format(struct device* dev) {
    ASSERT_NOT_NULL(dev);
}

/*
 * find first sector of cluster
 */
uint64_t fat_first_sector_of_cluster(uint32_t cluster, struct fat_fs_parameters* fs_parameters) {
    return ((cluster - 2) * fs_parameters->sectors_per_cluster) + fs_parameters->first_data_sector;
}

uint32_t fat_fat12_next_cluster(struct device* dev, uint32_t current_cluster, struct fat_fs_parameters* fs_parameters) {
    uint8_t FAT_table[fs_parameters->sector_size];
    uint32_t fat_offset = current_cluster + (current_cluster / 2);  // multiply by 1.5
    kprintf("fat_offset %llu\n", fat_offset);

    uint32_t fat_sector = fs_parameters->first_fat_sector + (fat_offset / fs_parameters->sector_size);
    kprintf("fat_sector %llu\n", fat_sector);

    uint32_t ent_offset = fat_offset % fs_parameters->sector_size;

    memset((uint8_t*)&FAT_table, 0, fs_parameters->sector_size);
    block_read(dev, fat_sector, (uint8_t*)&FAT_table, 1);

    unsigned short table_value = *(unsigned short*)&FAT_table[ent_offset];

    if (current_cluster & 0x0001) {
        table_value = table_value >> 4;
    } else {
        table_value = table_value & 0x0FFF;
    }
    return table_value;
}

uint32_t fat_fat16_next_cluster(struct device* dev, uint32_t current_cluster, struct fat_fs_parameters* fs_parameters) {
    uint8_t FAT_table[fs_parameters->sector_size];
    uint32_t fat_offset = current_cluster * 2;
    uint32_t fat_sector = fs_parameters->first_fat_sector + (fat_offset / fs_parameters->sector_size);
    uint32_t ent_offset = fat_offset % fs_parameters->sector_size;

    memset((uint8_t*)&FAT_table, 0, fs_parameters->sector_size);
    block_read(dev, fat_sector, (uint8_t*)&FAT_table, 1);

    return *(unsigned short*)&FAT_table[ent_offset];
}

struct fs_directory_listing* fat_list_dir(struct device* dev) {
    ASSERT_NOT_NULL(dev);

    struct fs_directory_listing* ret = fs_directory_listing_new();

    struct fat_fs_parameters fs_parameters;
    fat_read_fs_parameters(dev, &fs_parameters);

    if ((fs_parameters.type == FAT12) || (fs_parameters.type == FAT16)) {
        uint32_t current_sector = fs_parameters.first_root_dir_sector;

        bool more = true;
        while (more) {
            // read sector
            uint8_t* buffer = kmalloc(fs_parameters.sector_size);
            memset(buffer, 0, fs_parameters.sector_size);

            // read first sector of root dir
            block_read(dev, current_sector, buffer, 1);

            // loop entries
            for (uint16_t i = 0; i < fs_parameters.sector_size; i = i + sizeof(struct fat_dir)) {
                struct fat_dir* entry = (struct fat_dir*)&(buffer[i]);
                // entry ok
                if (entry->name[0] != 0) {
                    // entry is used
                    if (entry->name[0] != 0xe5) {
                        // not a long file name
                        if (entry->name[10] != 0xFF) {
                            kprintf("%s\n", entry->name);
                            //		struct fs_directory* dir = (struct fs_directory*) kmalloc(sizeof(struct fs_directory));
                            //		dir->flags=entry->attributes;
                            //		memcpy(dir->name, entry->name,11);
                            //			dir->name[12]=0;

                            // dir->name = name;

                            //		dir->size = entry->size;
                            //		arraylist_add(ret->lst,dir);
                            //		kprintf("FF%s\n",dir->name);
                        }
                    }
                } else {
                    // we done!
                    more = false;
                    break;
                }
            }
            kfree(buffer);
            current_sector = current_sector + 1;
        }
    } else {
        panic("Unsupported FAT type");
    }
    return ret;
}

// if (fs_parameters.type==FAT12){
// 	current_sector = current_sector +1;
// 	uint64_t next_cluster = fat_fat12_next_cluster(dev, current_cluster, &fs_parameters);
// 	kprintf("next cluster %llu\n",next_cluster);
// 	current_sector = fat_first_sector_of_cluster(1, &fs_parameters);
// 	kprintf("next sector %llu\n",current_sector);

// 	// blah
// //	current_sector=0;
// } else if (fs_parameters.type==FAT16){

// }

void fat_register() {
    struct fs_filesystem* fs = (struct fs_filesystem*)kmalloc(sizeof(struct fs_filesystem));
    fs->format = &fat_format;
    fs->list_dir = &fat_list_dir;
    fs->name = &fat_name;
    fs_register(fs);
}
