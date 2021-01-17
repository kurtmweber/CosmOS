//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RAMDISK_H
#define _RAMDISK_H

#include <types.h>
struct device;

/*
 * note that ramdisk uses the block api
 */
struct device* ramdisk_attach(uint16_t sector_size, uint16_t sector_count);
void ramdisk_detach(struct device* dev);

#endif
