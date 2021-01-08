//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <types.h>
#include <sys/deviceapi/deviceapi_block.h>

uint16_t block_get_sector_size(struct device* dev);
uint32_t block_get_total_size(struct device* dev);
void block_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count);
void block_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count);