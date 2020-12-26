//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ISADMA_H
#define _ISADMA_H

#include <types.h>

void isadma_devicemgr_register_devices();

/*
* device reading from memory
*/
void isadma_init_dma_read(uint8_t channel, uint64_t address, uint32_t len);
/*
* device writing to memory
*/
//void isadma_init_dma_write8(uint8_t channel, uint64_t address, uint32_t len);

#endif