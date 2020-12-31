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

typedef void (*ata_read_sector)(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size);
typedef void (*ata_write_sector)(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size);

struct deviceapi_ata {
    ata_read_sector read;
    ata_write_sector write;
};

#endif