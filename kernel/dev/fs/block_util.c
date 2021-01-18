//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/fs/block_util.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/devicemgr.h>

uint16_t block_get_sector_count(struct device* dev) {
    return block_get_total_size(dev) / block_get_sector_size(dev);
}

uint16_t block_get_sector_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    return (*block_api->sector_size)(dev);
}

uint32_t block_get_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    return (*block_api->total_size)(dev);
}

void block_write(struct device* dev, uint32_t sector, uint8_t* data) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);

    (*block_api->write)(dev, sector, data, 1);
}

/*
 * reads
 */
void block_read(struct device* dev, uint32_t sector, uint8_t* data) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    struct deviceapi_block* block_api = (struct deviceapi_block*)dev->api;
    ASSERT_NOT_NULL(block_api);
    (*block_api->read)(dev, sector, data, 1);
}