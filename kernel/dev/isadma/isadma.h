//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ISADMA_H
#define _ISADMA_H

#include <types.h>

#define ISA_DMA_64M             0x4000000                                // DMA chips can only use first 64M RAM
#define ISA_DMA_NUM_BUFFERS     0x08                                     // There are 8 channels
#define ISA_DMA_BUFFER_SIZE     0x2000                                   // 8k blocks
#define ISA_DMA_ALIGNMENT       0x10000                                  // channels must be 64k aligned
#define ISA_DMA_BUFSIZ          ISA_DMA_NUM_BUFFERS* ISA_DMA_BUFFER_SIZE // this is used by mm to reserve space

void isadma_devicemgr_register_devices();

/*
* device reading from memory
*/
void isadma_init_dma_read(uint8_t channel, uint32_t len);
/*
* device writing to memory
*/
void isadma_init_dma_write8(uint8_t channel, uint32_t);

/*
* get the DMA block for a channel
*/
uint64_t isadma_get_dma_block(uint8_t channel, uint32_t len);

/*
* location of the DMA buffers, set by the MM
*/
extern void *isadma_buf;

#endif