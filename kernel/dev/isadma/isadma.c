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
#include <panic/panic.h>

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

void isadma_read(uint8_t channel, uint64_t* address, uint16_t size) {

}

void isadma_write(uint8_t channel, uint64_t address, uint16_t size) {

}

uint16_t isadma_get_port(uint8_t channel){
	switch(channel){
		case 0:
			return ISA_DMA_CHAN03_START_ADDRESS_REGISTER_0_4;
		case 1:
			return ISA_DMA_CHAN03_START_ADDRESS_REGISTER_1_5;
		case 2:
			return ISA_DMA_CHAN03_START_ADDRESS_REGISTER_2_6;
		case 3:
			return ISA_DMA_CHAN03_START_ADDRESS_REGISTER_3_7;
		case 4:
			return ISA_DMA_CHAN47_START_ADDRESS_REGISTER_0_4;
		case 5:
			return ISA_DMA_CHAN47_START_ADDRESS_REGISTER_1_5;
		case 6:
			return ISA_DMA_CHAN47_START_ADDRESS_REGISTER_2_6;
		case 7:
			return ISA_DMA_CHAN47_START_ADDRESS_REGISTER_3_7;
		default:
			panic("Illegal ISA DMA channel");
	}
}
