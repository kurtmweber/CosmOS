//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VBLOCK_H
#define _VBLOCK_H

#include <types.h>

void vblock_devicemgr_register_devices();

// TODO replace this with a uniform interface
void vblock_read(uint32_t sector, uint8_t* target, uint32_t size);

#endif