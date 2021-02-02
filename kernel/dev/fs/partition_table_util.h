//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PARTITION_TABLE_UTIL_H
#define _PARTITION_TABLE_UTIL_H

#include <types.h>

struct device;

uint16_t partition_table_sector_size(struct device* partition_table_device, uint8_t partition_index);
uint32_t partition_table_total_size(struct device* partition_table_device, uint8_t partition_index);

/*
 * read 1 sector.  "data" is at least as big as sector_size
 */
void partition_table_write(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                           uint8_t* data);
/*
 * write 1 sector. "data" is at least as big as sector_size
 */
void partition_table_read(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                          uint8_t* data);

/*
 * write "count" sectors starting at lba "sector".  "data" is at least as big as sector_size
 */
void partition_table_write_sectors(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                                   uint8_t* data, uint32_t count);
/*
 * read "count" sectors starting at lba "sector".  "data" is at least as big as sector_size
 */
void partition_table_read_sectors(struct device* partition_table_device, uint8_t partition_index, uint32_t sector,
                                  uint8_t* data, uint32_t count);

#endif