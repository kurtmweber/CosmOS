//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RAMDISK_H
#define _RAMDISK_H

#include <types.h>

/*
 * note that ramdisk uses the block api
 */
void ramdisk_devicemgr_register_devices();

#endif
