//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ISADMA_H
#define _ISADMA_H

#include <types.h>

// https://wiki.osdev.org/DMA
// http://www.osdever.net/documents/dmaprogramming.pdf

// channels 0-3 (8 bit)
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_0_4 			0x00
#define ISA_DMA_CHAN03_COUNT_REGISTER_0_4 					0x01
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_1_5 			0x02
#define ISA_DMA_CHAN03_COUNT_REGISTER_1_5 					0x03
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_2_6 			0x04
#define ISA_DMA_CHAN03_COUNT_REGISTER_2_6 					0x05
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_3_7 			0x06
#define ISA_DMA_CHAN03_COUNT_REGISTER_3_7 					0x07
#define ISA_DMA_CHAN03_STATUS_REGISTER 						0x08
#define ISA_DMA_CHAN03_COMMAND_REGISTER 					0x08
#define ISA_DMA_CHAN03_REQUEST_REGISTER 					0x09
#define ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER 		0x0A
#define ISA_DMA_CHAN03_MODE_REGISTER 						0x0B
#define ISA_DMA_CHAN03_FLIP_FLOP_RESET_REGISTER 			0x0C
#define ISA_DMA_CHAN03_INTERMEDIATE_REGISTER 				0x0D
#define ISA_DMA_CHAN03_MASTER_RESET_REGISTER 				0x0D
#define ISA_DMA_CHAN03_MASK_RESET_REGISTER 					0x0E
#define ISA_DMA_CHAN03__MULTICHANNEL_MASK_RESET_REGISTER 	0x0F

// channels 4-7 (16 bit)
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_0_4 			0xC0
#define ISA_DMA_CHAN47_COUNT_REGISTER_0_4 					0xC2
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_1_5 			0xC4
#define ISA_DMA_CHAN47_COUNT_REGISTER_1_5 					0xC6
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_2_6 			0xC8
#define ISA_DMA_CHAN47_COUNT_REGISTER_2_6 					0xCA
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_3_7 			0xCC
#define ISA_DMA_CHAN47_COUNT_REGISTER_3_7 					0xCE
#define ISA_DMA_CHAN47_STATUS_REGISTER 						0xD0
#define ISA_DMA_CHAN47_COMMAND_REGISTER 					0xD0
#define ISA_DMA_CHAN47_REQUEST_REGISTER 					0xD2
#define ISA_DMA_CHAN47_SINGLE_CHANNEL_MASK_REGISTER 		0xD4
#define ISA_DMA_CHAN47_MODE_REGISTER 						0xD6
#define ISA_DMA_CHAN47_FLIP_FLOP_RESET_REGISTER 			0xD8
#define ISA_DMA_CHAN47_INTERMEDIATE_REGISTER 				0xDA
#define ISA_DMA_CHAN47_MASTER_RESET_REGISTER 				0xDA
#define ISA_DMA_CHAN47_MASK_RESET_REGISTER 					0xDC
#define ISA_DMA_CHAN47__MULTICHANNEL_MASK_RESET_REGISTER 	0xDE

// page address registers
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_0						0x87
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_1						0x83
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_2						0x81
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_3						0x82
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_4						0x8F
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_5						0x8B	
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_6						0x89
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_7						0x8A

// mode selection bits 7:6
#define ISA_DMA_DEMAND_MODE 								0x00 //00
#define ISA_DMA_SINGLE_MODE 								0x40 //01
#define ISA_DMA_BLOCK_MODE 								`   0x80 //10
#define ISA_DMA_CASCADE_MODE 								0xC0 //11

// address increment / decrement bit 5
#define ISA_DMA_ADDRESS_DECREMENT 							0x20 //1
#define ISA_DMA_ADDRESS_INCREMENT 							0x00 //0
 
// autoInitialization enable bit 4
#define ISA_DMA_AUTO_INIT 									0x10 //1
#define ISA_DMA_SINGLE_CYCLE 								0x00 //0
 
// transfer type bits 3:2
#define ISA_DMA_VERIFY_TRANSFER 							0x00 //00
#define ISA_DMA_WRITE_TRANSFER 							    0x04 //01
#define ISA_DMA_READ_TRANSFER 								0x08 //10

void isadma_devicemgr_register_devices();

void isadma_read(uint8_t channel, uint64_t* address, uint16_t size);
void isadma_write(uint8_t channel, uint64_t address, uint16_t size);

#endif