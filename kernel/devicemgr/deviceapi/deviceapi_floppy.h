//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file defines the interface that all floppy devices will implement
*/
#ifndef _DEVICEAPI_FLOPPY_H
#define _DEVICEAPI_FLOPPY_H

#include <types.h>
#include <devicemgr/devicemgr.h>

typedef void (*floppy_read_sector)(struct device* dev, uint32_t sector, uint8_t* data, uint8_t* size);
typedef void (*floppy_write_sector)(struct device* dev, uint32_t sector, uint8_t* data, uint8_t* size);

struct deviceapi_floppy {
    floppy_read_sector read;
    floppy_write_sector write;
};

#endif