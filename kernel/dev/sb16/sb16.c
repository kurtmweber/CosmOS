/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Tom Everett                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/sb16/sb16.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <collection/list/list.h>
#include <interrupts/interrupt_router.h>
#include <devicemgr/deviceapi/deviceapi_dsp.h>

// https://wiki.osdev.org/Sound_Blaster_16

#define SB16_IRQ        5
#define SB16_DMA1       1
#define SB16_DMA2       5

// ports
#define SB16_PORT_MIXER 			0x04
#define SB16_PORT_MIXER_DATA 		0x05
#define SB16_PORT_RESET 			0x06
#define SB16_PORT_READ 				0x0A
#define SB16_PORT_WRITE	 			0x0C
#define SB16_PORT_READ_STATUS 		0x0E
#define SB16_PORT_INT_ACK 			0x0F

// commands (DSP)
#define SB16_COMMAND_SET_TIME_CONSTANT 			0x40
#define SB16_COMMAND_SET_OUTPUT_SAMPLE_RATE 	0x41
#define SB16_COMMAND_SPEAKER_ON 				0xD1
#define SB16_COMMAND_SPEAKER_OFF 				0xD3
#define SB16_COMMAND_STOP_8_BIT 				0xD0
#define SB16_COMMAND_RESUME_8_BIT 				0xD4
#define SB16_COMMAND_STOP_16_BIT 				0xD5
#define SB16_COMMAND_RESUME_16_BIT 				0xD6
#define SB16_COMMAND_GET_DSP_VERSION 			0xE1

// commands (MIXER)
#define SB16_COMMAND_MASTER_VOLUME				0x22
#define SB16_COMMAND_IRQ 						0x80

/*
* device parameters for an sb16
*/
struct sb16_devicedata {
    uint32_t port;
	uint8_t irq;
} __attribute__((packed));

void sb16_handle_irq(stackFrame *frame) {
}

/*
* perform device instance specific init here
*/
void deviceInitSB16(struct device* dev){
	struct sb16_devicedata* sb16_data = (struct sb16_devicedata*) dev->deviceData;
    kprintf("Init %s at IRQ %llu Port %#X (%s)\n",dev->description, sb16_data->irq, sb16_data->port, dev->name);
    interrupt_router_register_interrupt_handler(sb16_data->irq, &sb16_handle_irq);
}

void sb16_devicemgr_register_device(uint64_t port){
   /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "Soundblaster 16");
	deviceinstance->devicetype = DSP;
	deviceinstance->init =  &deviceInitSB16;
	/*
	* device api
	*/
	struct deviceapi_dsp* api = (struct deviceapi_dsp*) kmalloc (sizeof(struct deviceapi_dsp));
	deviceinstance->api = api;
	/*
	* device data
	*/
	struct sb16_devicedata* deviceData = (struct sb16_devicedata*) kmalloc(sizeof(struct sb16_devicedata));
	deviceData->port = port;
	deviceData->irq = SB16_IRQ;
	deviceinstance->deviceData = deviceData;
	/*
	* register
	*/
	devicemgr_register_device(deviceinstance);
}

/*
* https://wiki.osdev.org/Sound_Blaster_16#Reseting_DSP_.28detecting_DSP.29
*/
bool sb16_detect(uint32_t port){
	asm_out_b(port+SB16_PORT_RESET,0x01);
	asm_out_b(port+SB16_PORT_RESET,0x00);
	uint8_t x = 0x00;
	do{
		x = asm_in_b(port+SB16_PORT_READ_STATUS);
	} while (0==(x & 0x80));
	x = asm_in_b(port+SB16_PORT_READ);
	if (x==0xaa){
		return true;
	}
	return false;
}

void sb16_devicemgr_register_devices(){
	if (sb16_detect(0x220)){
		sb16_devicemgr_register_device(0x220);
	}
	if (sb16_detect(0x240)){
		sb16_devicemgr_register_device(0x240);
	}
	if (sb16_detect(0x260)){
		sb16_devicemgr_register_device(0x260);
	}
}

