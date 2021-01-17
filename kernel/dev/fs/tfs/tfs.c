//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <dev/fs/tfs/tfs.h>
#include <dev/fs/tfs/tfs_block.h>
#include <dev/fs/tfs/tfs_dir.h>
#include <dev/fs/tfs/tfs_map.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

/*
 * format. I just guessed here.
 */
void tfs_format(struct device* dev) {
    /*
     * figure out how many map blocks we need
     */
    uint32_t number_map_blocks = tfs_map_block_count(dev);
    /*
     * create superblock
     */
    struct tfs_superblock_block superblock;
    memset((uint8_t*)&superblock, 0, sizeof(struct tfs_superblock_block));
    superblock.magic = DFS_MAGIC_SUPERBLOCK;
    superblock.blocks_size = (uint64_t)block_get_sector_size(dev);
    superblock.blocks_count = (uint64_t)block_get_sector_count(dev);
    superblock.number_map_blocks = number_map_blocks;
    superblock.root_dir = number_map_blocks + 1;  // sector one, since sector zero is the super-block
    kprintf("blocks_count %llu\n", superblock.blocks_count);
    kprintf("number_map_blocks %llu\n", superblock.number_map_blocks);
    kprintf("root_dir %llu\n", superblock.root_dir);
    /*
     * write superblock
     */
    tfs_write_superblock(dev, &superblock);
    /*
     * create & write map blocks
     */
    for (uint32_t i = 0; i < number_map_blocks; i++) {
        struct tfs_map_block map_block;
        memset((uint8_t*)&map_block, 0, sizeof(struct tfs_map_block));
        tfs_write_map_block(dev, &map_block, i + 1);
        //    kprintf("map block: %llu\n",i+1);
    }
    /*
     * root dir block
     */
    struct tfs_dir_block root_dir_block;
    memset((uint8_t*)&root_dir_block, 0, sizeof(struct tfs_dir_block));
    root_dir_block.next = 0;
    /*
     * write root dir
     */
    tfs_write_dir_block(dev, &root_dir_block, superblock.root_dir);
    kprintf("dir block: %llu\n", superblock.root_dir);
}

bool tfs_dir_list_iterator(struct tfs_file_block* file_block) {
    ASSERT_NOT_NULL(file_block);
    kprintf("dfs file at block %s\n", file_block->name);
    return true;
}

struct fs_directory_listing* tfs_list_dir(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    tfs_dir_iterate_files(dev, &tfs_dir_list_iterator);
}

void tfs_read(struct device* dev, const uint8_t* name, const uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(data);
    ASSERT(strlen(name) < DFS_FILENAME_SIZE);
}

void tfs_write(struct device* dev, const uint8_t* name, const uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(data);
    ASSERT(strlen(name) < DFS_FILENAME_SIZE);

    kprintf("write file: %s of length %llu\n", name, size);
    /*
     * find an empty spot in the root dir
     */

    /*
     * write to the dir
     */

    /*
     * make an allocation block
     */
}

/*
 * perform device instance specific init here
 */
void tfs_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void tfs_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
}

struct device* tfs_attach(struct device* block_device) {
    ASSERT(sizeof(struct tfs_superblock_block) == DFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_dir_block) == DFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_file_block) == DFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_file_allocation_block) == DFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_map_block) == DFS_BLOCK_SIZE);

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &tfs_init;
    deviceinstance->uninit = &tfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    devicemgr_set_device_description(deviceinstance, "Trivial File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    api->format = &tfs_format;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_attach_device(deviceinstance);

    /*
     * return device
     */
    return deviceinstance;
}

void tfs_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}