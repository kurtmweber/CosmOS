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
#include <panic/panic.h>
#include <sleep/sleep.h>
#include <dev/isadma/isadma.h>

// https://wiki.osdev.org/Sound_Blaster_16

#define SB16_IRQ        5
#define SB16_DMA       1

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

// transfer modes
#define SB16_TRANSFER_8BIT						0xC0 // bits 7-4
#define SB16_TRANSFER_16BIT						0xB0 // bits 7-4
#define SB16_TRANSFER_PLAYING					0x00 // bit 3
#define SB16_TRANSFER_RECORDING					0x04 // bit 3
#define SB16_TRANSFER_FIFO_ON					0x01 // bit 1
#define SB16_TRANSFER_FIFO_OFF					0x00 // bit 1

/*
* device parameters for an sb16
*/
struct sb16_devicedata {
    uint32_t port;
	uint8_t irq;
} __attribute__((packed));

void sb16_handle_irq(stackFrame *frame) {
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
	kprintf("S");
}

/*
* perform device instance specific init here
*/
void deviceInitSB16(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct sb16_devicedata* sb16_data = (struct sb16_devicedata*) dev->deviceData;
    kprintf("Init %s at IRQ %llu Port %#X (%s)\n",dev->description, sb16_data->irq, sb16_data->port, dev->name);
    interrupt_router_register_interrupt_handler(sb16_data->irq, &sb16_handle_irq);
}

void sb16_reset(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct sb16_devicedata* sb16_data = (struct sb16_devicedata*) dev->deviceData;
	asm_out_b(sb16_data->port+SB16_PORT_RESET, 0x01);
	sleep_wait(100);
	asm_out_b(sb16_data->port+SB16_PORT_RESET, 0x0);
}

void sb16_speaker_on(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct sb16_devicedata* sb16_data = (struct sb16_devicedata*) dev->deviceData;
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, SB16_COMMAND_SPEAKER_ON);
}

void sb16_speaker_off(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct sb16_devicedata* sb16_data = (struct sb16_devicedata*) dev->deviceData;
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, SB16_COMMAND_SPEAKER_OFF);
}

void sb16_init_dma(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct sb16_devicedata* sb16_data = (struct sb16_devicedata*) dev->deviceData;
	// disable channel 1 (number of channel + 0x04)
	asm_out_b(ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER, 0x05);

	// reset flip flip
	asm_out_b(ISA_DMA_CHAN03_FLIP_FLOP_RESET_REGISTER, 0x01);
	
	// transfer mode
	asm_out_b(ISA_DMA_CHAN03_MODE_REGISTER, 0x49);

	// PAGE TRANSFER (EXAMPLE POSITION IN MEMORY 0x[01]0F04) - SET THIS VALUE FOR YOU
	asm_out_b(ISA_DMA_CHANNEL_PAGE_ADDRESS_1, 0x01);

	// POSITION LOW BIT (EXAMPLE POSITION IN MEMORY 0x010F[04]) - SET THIS VALUE FOR YOU
	asm_out_b(ISA_DMA_CHAN03_START_ADDRESS_REGISTER_1_5, 0x04);

	// POSITON HIGH BIT (EXAMPLE POSITION IN MEMORY 0x01[0F]04) - SET THIS VALUE FOR YOU
	asm_out_b(ISA_DMA_CHAN03_START_ADDRESS_REGISTER_1_5, 0x0F);

	// COUNT LOW BIT (EXAMPLE 0x0FFF) - SET THIS VALUE FOR YOU
	asm_out_b(ISA_DMA_CHAN03_COUNT_REGISTER_1_5, 0xFF);

	// COUNT HIGH BIT (EXAMPLE 0x0FFF) - SET THIS VALUE FOR YOU
	asm_out_b(ISA_DMA_CHAN03_COUNT_REGISTER_1_5, 0x0F);

	// enable channel 1
	asm_out_b(ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER, 0x01);
}

/*
* https://wiki.osdev.org/Sound_Blaster_16
*/
// ;SOUND BLASTER 16 driver in real mode
 
//   ;reset sound blaster
//   OUTB 0x226, 1 ;reset port
//   mov ah, 86h
//   mov cx, 0x0000
//   mov dx, 0xFFFF
//   int 15h ;wait
//   OUTB 0x226, 0 ;reset port
 
//   ;turn speaker on
//   OUTB 0x22C, 0xD1
 
//   ;DMA channel 1
//   OUTB 0x0A, 5 ;disable channel 1 (number of channel + 0x04)
//   OUTB 0x0C, 1 ;flip flop
//   OUTB 0x0B, 0x49 ;transfer mode
//   OUTB 0x83, 0x01 ;PAGE TRANSFER (EXAMPLE POSITION IN MEMORY 0x[01]0F04) - SET THIS VALUE FOR YOU
//   OUTB 0x02, 0x04 ;POSITION LOW BIT (EXAMPLE POSITION IN MEMORY 0x010F[04]) - SET THIS VALUE FOR YOU
//   OUTB 0x02, 0x0F ;POSITON HIGH BIT (EXAMPLE POSITION IN MEMORY 0x01[0F]04) - SET THIS VALUE FOR YOU
//   OUTB 0x03, 0xFF ;COUNT LOW BIT (EXAMPLE 0x0FFF) - SET THIS VALUE FOR YOU
//   OUTB 0x03, 0x0F ;COUNT HIGH BIT (EXAMPLE 0x0FFF) - SET THIS VALUE FOR YOU
//   OUTB 0x0A, 1 ;enable channel 1
 
//   ;program sound blaster 16
//   OUTB 0x22C, 0x40 ;set time constant
//   OUTB 0x22C, 165 ;10989 Hz
//   OUTB 0x22C, 0xC0 ;8 bit sound
//   OUTB 0x22C, 0x00 ;mono and unsigned sound data
//   OUTB 0x22C, 0xFE ;COUNT LOW BIT - COUNT LENGHT-1 (EXAMPLE 0x0FFF SO 0x0FFE) - SET THIS VALUE FOR YOU
//   OUTB 0x22C, 0x0F ;COUNT HIGH BIT - COUNT LENGHT-1 (EXAMPLE 0x0FFF SO 0x0FFE) - SET THIS VALUE FOR YOU
 
//   ;now transfer start - dont forget to handle irq
void play(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct sb16_devicedata* sb16_data = (struct sb16_devicedata*) dev->deviceData;

	sb16_reset(dev);
	sb16_speaker_on(dev);
	sb16_init_dma(dev);

	// set time constant
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, SB16_COMMAND_SET_TIME_CONSTANT);
	
	// 10989 Hz
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 165);

	// 8 bit sound
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, SB16_TRANSFER_8BIT);

	// mono and unsigned sound data
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0x00);

	// COUNT LOW BIT - COUNT LENGHT-1 (EXAMPLE 0x0FFF SO 0x0FFE) - SET THIS VALUE FOR YOU
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0xFE);

	// COUNT HIGH BIT - COUNT LENGHT-1 (EXAMPLE 0x0FFF SO 0x0FFE) - SET THIS VALUE FOR YOU
	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0x0F);
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
	api->play=play;
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

