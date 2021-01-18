//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BLOCK_UTIL_H
#define _BLOCK_UTIL_H

#include <sys/deviceapi/deviceapi_block.h>
#include <types.h>

uint16_t block_get_sector_size(struct device* dev);
uint16_t block_get_sector_count(struct device* dev);
uint32_t block_get_total_size(struct device* dev);

/*
 * read "count" sectors starting at lba "sector".  "data" is at least as big as count*sectot_size
 */
void block_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count);
/*
 * write "count" sectors starting at lba "sector".  "data" is at least as big as count*sectot_size
 */

void block_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count);

#endif