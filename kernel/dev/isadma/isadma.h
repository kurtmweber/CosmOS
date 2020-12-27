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
void isadma_init_dma_read(uint8_t channel, uint16_t len);
/*
* device writing to memory
*/
void isadma_init_dma_write8(uint8_t channel, uint16_t);

/*
* this is a horrible hack, for now. ISA DMA can only take place
* in the lower 64MB of RAM and the memory block can't cross
* a 16MB boundary.  This function returns such a block.  Really, it
* should delegate to a function in mm.h
*/
uint64_t isadma_get_dma_block(uint8_t channel, uint16_t len);

extern void *isadma_buf;

#endif