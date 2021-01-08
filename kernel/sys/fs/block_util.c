//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BLOCK_UTIL_H
#define _BLOCK_UTIL_H

#include <sys/fs/block_util.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/debug/assert.h>

uint16_t block_get_sector_size(struct device* dev){
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
	struct deviceapi_block* block_api = (struct deviceapi_block*) dev->api;
    ASSERT_NOT_NULL(block_api, "block_api cannot be null");    

    return (*block_api->sector_size)(dev);
}

uint32_t block_get_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
	struct deviceapi_block* block_api = (struct deviceapi_block*) dev->api;
    ASSERT_NOT_NULL(block_api, "block_api cannot be null");    

    return (*block_api->total_size)(dev);
}

void block_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(data, "data cannot be null");    
	struct deviceapi_block* block_api = (struct deviceapi_block*) dev->api;
    ASSERT_NOT_NULL(block_api, "block_api cannot be null");    

    (*block_api->write)(dev, sector, data, count);
}

void block_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(data, "data cannot be null");    
	struct deviceapi_block* block_api = (struct deviceapi_block*) dev->api;
    ASSERT_NOT_NULL(block_api, "block_api cannot be null");    
    (*block_api->read)(dev, sector, data, count);
}


#endif