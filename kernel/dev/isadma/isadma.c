//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/isadma/isadma.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <panic/assert.h>

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

// channel, bits 1:0
#define ISA_DMA_CHANNEL_0_4 								0x00 //00
#define ISA_DMA_CHANNEL_1_5								    0x01 //01
#define ISA_DMA_CHANNEL_2_6								    0x02 //10
#define ISA_DMA_CHANNEL_3_7								    0x03 //11

struct isa_dma_channel_parameters {
	uint32_t DMAAddressPort;
	uint32_t DMACountPort;
	uint32_t DMAPagePort;
	uint32_t DMAMaskReg;
	uint32_t DMAClearReg;
	uint32_t DMAModeReg;
	bool eightbit;	
	uint8_t  DMAChannelFlags; // bits 0:1 in channel mode registers
};

/*
* perform device instance specific init here
*/
void deviceInitISADMA(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s (%s)\n",dev->description, dev->name);
}

void isadma_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "8237 ISA DMA");
	deviceinstance->devicetype = DMA;
	deviceinstance->init =  &deviceInitISADMA;
	devicemgr_register_device(deviceinstance);
}

/*
* https://wiki.osdev.org/ISA_DMA#The_Registers
*/
/*
* set DMA channel 2 to transfer data from 0x1000 - 0x33ff in memory
* paging must map this _physical_ memory elsewhere and _pin_ it from paging to disk!
* set the counter to 0x23ff, the length of a track on a 1.44 MiB floppy - 1 (assuming 512 byte sectors)
* transfer length = counter + 1
*/
//    out 0x0a, 0x06      ; mask DMA channel 2 and 0 (assuming 0 is already masked)
//    out 0x0c, 0xFF      ; reset the master flip-flop
//    out 0x04, 0         ; address to 0 (low byte)
//    out 0x04, 0x10      ; address to 0x10 (high byte)
//    out 0x0c, 0xFF      ; reset the master flip-flop (again!!!)
//    out 0x05, 0xFF      ; count to 0x23ff (low byte)
//    out 0x05, 0x23      ; count to 0x23ff (high byte),
//    out 0x81, 0         ; external page register to 0 for total address of 00 10 00
//    out 0x0a, 0x02      ; unmask DMA channel 2
//    ret
void isadma_initialize_floppy_DMA() {
	// mask DMA channel 2 and 0 (assuming 0 is already masked)
	asm_out_b(ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER, 0x06);
	// reset the master flip-flop
	asm_out_b(ISA_DMA_CHAN03_FLIP_FLOP_RESET_REGISTER, 0xFF);
	// address to 0 (low byte)
	asm_out_b(ISA_DMA_CHAN03_START_ADDRESS_REGISTER_2_6, 0x00);
	// address to 0x10 (high byte)
	asm_out_b(ISA_DMA_CHAN03_START_ADDRESS_REGISTER_2_6, 0x10);
	// reset the master flip-flop (again!!!)
	asm_out_b(ISA_DMA_CHAN03_FLIP_FLOP_RESET_REGISTER, 0xFF);
	// count to 0x23ff (low byte)
	asm_out_b(ISA_DMA_CHAN03_COUNT_REGISTER_2_6, 0xFF);
	// count to 0x23ff (high byte),
	asm_out_b(ISA_DMA_CHAN03_COUNT_REGISTER_2_6, 0x23);
	// external page register to 0 for total address of 00 10 00
	asm_out_b(ISA_DMA_CHANNEL_PAGE_ADDRESS_2, 0x00);
	// unmask DMA channel 2
	asm_out_b(ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

/*
* get the appropriate DMA ports for a channel
*/
// http://www.osdever.net/documents/dmaprogramming.pdf
void getDMAParameters(uint8_t channel, struct isa_dma_channel_parameters* parameters){
	ASSERT(channel>=0, "channel must be greater than or equal to 0");
	ASSERT(channel<=7, "channel must be less than or equal to 7");

	switch(channel){
		case 0:
			parameters->DMAAddressPort=ISA_DMA_CHAN03_START_ADDRESS_REGISTER_0_4;
			parameters->DMACountPort=ISA_DMA_CHAN03_COUNT_REGISTER_0_4;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_0;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_0_4;
			break;
		case 1:
			parameters->DMAAddressPort=ISA_DMA_CHAN03_START_ADDRESS_REGISTER_1_5;
			parameters->DMACountPort=ISA_DMA_CHAN03_COUNT_REGISTER_1_5;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_1;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_1_5;
			break;
		case 2:
			parameters->DMAAddressPort=ISA_DMA_CHAN03_START_ADDRESS_REGISTER_2_6;
			parameters->DMACountPort=ISA_DMA_CHAN03_COUNT_REGISTER_2_6;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_2;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_2_6;
			break;
		case 3:
			parameters->DMAAddressPort=ISA_DMA_CHAN03_START_ADDRESS_REGISTER_3_7;
			parameters->DMACountPort=ISA_DMA_CHAN03_COUNT_REGISTER_3_7;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_3;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_3_7;
			break;
		case 4:
			parameters->DMAAddressPort=ISA_DMA_CHAN47_START_ADDRESS_REGISTER_0_4;
			parameters->DMACountPort=ISA_DMA_CHAN47_COUNT_REGISTER_0_4;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_4;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_0_4;
			break;
		case 5:
			parameters->DMAAddressPort=ISA_DMA_CHAN47_START_ADDRESS_REGISTER_1_5;
			parameters->DMACountPort=ISA_DMA_CHAN47_COUNT_REGISTER_1_5;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_5;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_1_5;
			break;
		case 6:
			parameters->DMAAddressPort=ISA_DMA_CHAN47_START_ADDRESS_REGISTER_2_6;
			parameters->DMACountPort=ISA_DMA_CHAN47_COUNT_REGISTER_2_6;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_6;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_2_6;
			break;
		case 7:
			parameters->DMAAddressPort=ISA_DMA_CHAN47_START_ADDRESS_REGISTER_3_7;
			parameters->DMACountPort=ISA_DMA_CHAN47_COUNT_REGISTER_3_7;
			parameters->DMAPagePort=ISA_DMA_CHANNEL_PAGE_ADDRESS_7;
			parameters->DMAChannelFlags=ISA_DMA_CHANNEL_3_7;
			break;
	}
	if (channel <4){
		parameters->DMAMaskReg=ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER;
		parameters->DMAClearReg=ISA_DMA_CHAN03_FLIP_FLOP_RESET_REGISTER;
		parameters->DMAModeReg=ISA_DMA_CHAN03_MODE_REGISTER;
		parameters->eightbit=1;
	} else {
		parameters->DMAMaskReg=ISA_DMA_CHAN47_SINGLE_CHANNEL_MASK_REGISTER;
		parameters->DMAClearReg=ISA_DMA_CHAN47_FLIP_FLOP_RESET_REGISTER;
		parameters->DMAModeReg=ISA_DMA_CHAN47_MODE_REGISTER;
		parameters->eightbit=0;
	}
}

/*
* this function shared by read and write
*/
void isadma_init_dma(uint8_t channel, uint64_t address, uint32_t len, uint8_t rw_mode) {
//	kprintf("channel %#X\n", channel);

	ASSERT(channel>=0, "channel must be greater than or equal to 0");
	ASSERT(channel<=7, "channel must be less than or equal to 7");
//	ASSERT(channel!=0, "DMA channel 0 is unusable (RAM refresh channel)");
//	ASSERT(channel!=4, "DMA channel 4 is unusable (cascade channel)");

	uint16_t page = address >> 16;
	uint16_t buffer = address - (page << 16);
	struct isa_dma_channel_parameters channel_parameters;
	getDMAParameters(channel, &channel_parameters);

//	kprintf("DMAAddressPort %#X\n", channel_parameters.DMAAddressPort);
//	kprintf("DMACountPort %#X\n", channel_parameters.DMACountPort);
//	kprintf("DMAPagePort %#X\n", channel_parameters.DMAPagePort);

//	kprintf("page %#X\n", page);
//	kprintf("buffer %#X\n", buffer);

	// disable channel (number of channel + 0x04)
	asm_out_b(channel_parameters.DMAMaskReg, channel+0x04);

	// reset flip-flop
	asm_out_b(channel_parameters.DMAClearReg, 0x01);
	
	// transfer mode
	uint8_t mode=ISA_DMA_SINGLE_MODE|ISA_DMA_ADDRESS_INCREMENT|ISA_DMA_SINGLE_CYCLE|rw_mode|channel_parameters.DMAChannelFlags;
	asm_out_b(channel_parameters.DMAModeReg, mode);

	// PAGE TRANSFER
	asm_out_b(channel_parameters.DMAPagePort, page);

	// POSITION LOW BITBYTE
	asm_out_b(channel_parameters.DMAAddressPort, LOW_OF_W(buffer));

	// POSITON HIGH BITBYTE
	asm_out_b(channel_parameters.DMAAddressPort, HIGH_OF_W(buffer));

	// COUNT LOW BYTE
	asm_out_b(channel_parameters.DMACountPort, LOW_OF_W(len-1));

	// COUNT HIGH BYTE
	asm_out_b(channel_parameters.DMACountPort, HIGH_OF_W(len-1));

	// enable channel 1
	asm_out_b(channel_parameters.DMAMaskReg, ISA_DMA_CHANNEL_1_5);
}

void isadma_init_dma_read(uint8_t channel, uint64_t address, uint32_t len) {
	isadma_init_dma(channel, address, len, ISA_DMA_READ_TRANSFER);
}

void isadma_init_dma_write(uint8_t channel, uint64_t address, uint32_t len) {
	isadma_init_dma(channel, address, len, ISA_DMA_WRITE_TRANSFER);
}

