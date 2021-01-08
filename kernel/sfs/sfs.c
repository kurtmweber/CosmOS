//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sfs/sfs.h>
#include <sys/console/console.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/string/mem.h>

deviceType CompatibleDeviceTypes[] = {ATA, RAMDISK, VBLOCK};

bool sfs_check_device_compatibility(deviceType dt){
    for (int i=0; i<3; i++){
        if (CompatibleDeviceTypes[i]==dt){
            return true;
        }
    }
    return false;
} 

void sfs_format(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");

    bool compatible_device = sfs_check_device_compatibility(dev->devicetype);
    if (false == compatible_device){
        panic("Incompatible device");
    }

    struct deviceapi_block* block_api = (struct deviceapi_block*) dev->api;

    // device parameters
    uint64_t total_size = (*block_api->total_size)(dev);
    uint32_t sector_size = (*block_api->sector_size)(dev);
    uint32_t total_sectors = total_size / sector_size;

    // allocate a superblock struct
    struct sfs_superblock* superblock = kmalloc(sizeof(struct sfs_superblock));
    memset((uint8_t*)superblock,0,sizeof(struct sfs_superblock));
    superblock->timestamp=0; // later
    superblock->dataarea_size_blocks = total_sectors-2; // 1 for superblock and 1 for index
    superblock->indexarea_size_bytes = sector_size; // 1 sector
    superblock->reserved_blocks=1; // 1, for the superblock
    superblock->total_blocks=total_sectors;
    superblock->version=0x10;  // 1.0
    superblock->magic[0]=0x53;
    superblock->magic[1]=0x46;
    superblock->magic[2]=0x53;
    superblock->block_size = (sector_size/512)+1;

    // write superblock
    (*block_api->write)(dev, 0,(uint8_t*)superblock, sizeof(struct sfs_superblock));
}
