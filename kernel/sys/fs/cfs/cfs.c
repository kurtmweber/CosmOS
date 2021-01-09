//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/cfs/cfs.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/fs/block_util.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/debug.h>
#include <sys/string/mem.h>

#define CFS_MAGIC_SUPERBLOCK 0x303244594C4F4647
#define CFS_MAGIC_DATA 0x30324D4C424D4C42
#define CFS_MAGIC_PRESENTATION 0x4E45444942454F4A
#define CFS_MAGIC_GROUP 0x535241484E444942

struct cfs_superblock {
    uint64_t magic;
    uint64_t lastmount;
    uint64_t primary_data_space;
    uint64_t primary_presentation_space;
    uint64_t primary_group_directory;
    uint64_t reserved[58];
} __attribute__((packed));

struct cfs_directory {
    uint64_t magic;
    uint64_t previous;
    uint64_t next;
    uint64_t pointer[61];
} __attribute__((packed));

void cfs_list_dir(struct device* dev, struct list* lst) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(lst, "lst cannot be null");

    uint16_t sector_size = block_get_sector_size(dev);
    kprintf("sector size: %llu\n", sector_size);

    uint32_t total_size = block_get_total_size(dev);
    kprintf("total size: %llu\n", total_size);

    uint8_t* buffer = kmalloc(sector_size);
	memset(buffer, 0, sector_size);

    block_read(dev, 0, buffer,1);
}

/*
* read the superblock at lba 0
*/
void cfs_read_superblock(struct device* dev, struct cfs_superblock* superblock){
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(superblock, "superblock cannot be null");    

    block_read(dev, 0, (uint8_t*) superblock,1);
}

/*
* write the superblock at lba 0
*/
void cfs_write_superblock(struct device* dev, struct cfs_superblock* superblock){
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(superblock, "superblock cannot be null");    

    block_write(dev, 0, (uint8_t*) superblock,1);
}

/*
* format.  I just guessed here. I wrote 1x superblock and 
*/
void cfs_format(struct device* dev, struct cfs_superblock* superblock) {

}

