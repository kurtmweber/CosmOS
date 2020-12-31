//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file defines the interface that all ATA devices will implement
*/
#ifndef _DEVICEAPI_ATA_H
#define _DEVICEAPI_ATA_H

#include <types.h>
#include <devicemgr/devicemgr.h>

typedef void (*ata_read_sector_function)(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size);
typedef void (*ata_write_sector_function)(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size);
typedef uint16_t (*ata_sector_size_function)(struct device* dev);
typedef uint32_t (*ata_total_sectors_function)(struct device* dev);

struct deviceapi_ata {
    ata_read_sector_function read;
    ata_write_sector_function write;
    ata_sector_size_function sector_size;
    ata_total_sectors_function total_sectors;
};

#endif