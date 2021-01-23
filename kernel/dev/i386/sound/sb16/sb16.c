/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/i386/isadma/isadma.h>
#include <dev/i386/sound/sb16/sb16.h>
#include <sys/asm/asm.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_dsp.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <sys/string/mem.h>

// https://wiki.osdev.org/Sound_Blaster_16

#define SB16_IRQ 5
#define SB16_DMA_8BIT 1
#define SB16_DMA_16BIT 5

// ports
#define SB16_PORT_MIXER 0x04
#define SB16_PORT_MIXER_DATA 0x05
#define SB16_PORT_RESET 0x06
#define SB16_PORT_READ 0x0A
#define SB16_PORT_WRITE 0x0C
#define SB16_PORT_READ_STATUS 0x0E
#define SB16_PORT_INT_ACK 0x0F

// commands (DSP)
#define SB16_COMMAND_SET_TIME_CONSTANT 0x40
#define SB16_COMMAND_SET_OUTPUT_SAMPLE_RATE 0x41
#define SB16_COMMAND_SPEAKER_ON 0xD1
#define SB16_COMMAND_SPEAKER_OFF 0xD3
#define SB16_COMMAND_STOP_8_BIT 0xD0
#define SB16_COMMAND_RESUME_8_BIT 0xD4
#define SB16_COMMAND_STOP_16_BIT 0xD5
#define SB16_COMMAND_RESUME_16_BIT 0xD6
#define SB16_COMMAND_GET_SPEAKER_STATUS 0xD8
#define SB16_COMMAND_GET_DSP_VERSION 0xE1

// commands (MIXER)
#define SB16_COMMAND_MASTER_VOLUME 0x22
#define SB16_COMMAND_IRQ 0x80

// transfer modes
#define SB16_TRANSFER_8BIT 0xC0       // bits 7-4
#define SB16_TRANSFER_16BIT 0xB0      // bits 7-4
#define SB16_TRANSFER_PLAYING 0x00    // bit 3
#define SB16_TRANSFER_RECORDING 0x04  // bit 3
#define SB16_TRANSFER_FIFO_ON 0x01    // bit 1
#define SB16_TRANSFER_FIFO_OFF 0x00   // bit 1

uint16_t SB_BASE_PORTS[] = {0x220, 0x240, 0x260, 0x280};

// forward declarations
uint8_t sb16_get_dsp_version(struct device* dev);

/*
 * device parameters for an sb16
 */
struct sb16_devicedata {
    uint32_t port;
    uint8_t irq;
    uint8_t dsp_version;
} __attribute__((packed));

/*
 * interrupt handler
 */
void sb16_handle_irq(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("SSSSSSSSSSSSSSS");
}

/*
 * perform device instance specific init here
 */
void sb16_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);
    kprintf("Init %s at IRQ %llu Port %#llX (%s)\n", dev->description, sb16_data->irq, sb16_data->port, dev->name);
    interrupt_router_register_interrupt_handler(sb16_data->irq, &sb16_handle_irq);
    sb16_data->dsp_version = sb16_get_dsp_version(dev);
    kprintf("   DSP Version: %llu\n", sb16_data->dsp_version);
}

/*
 * stereo
 */
uint8_t sb16_set_stereo_output(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);
    /*
     * set stero
     */
    asm_out_b(sb16_data->port + SB16_PORT_MIXER, 0x0e);
    uint8_t temp = asm_in_b(sb16_data->port + SB16_PORT_MIXER_DATA);
    asm_out_b(sb16_data->port + SB16_PORT_MIXER_DATA, (temp | 0x2));
}

/*
 * get the speaker status
 */
uint8_t sb16_get_speaker_status(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);

    asm_out_b(sb16_data->port + SB16_PORT_WRITE, SB16_COMMAND_GET_SPEAKER_STATUS);
    return asm_in_b(sb16_data->port + SB16_PORT_READ);
}

/*
 * get the DSP version
 */
uint8_t sb16_get_dsp_version(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);

    asm_out_b(sb16_data->port + SB16_PORT_WRITE, SB16_COMMAND_GET_DSP_VERSION);
    return asm_in_b(sb16_data->port + SB16_PORT_READ);
}

/*
 * reset the device
 */
void sb16_reset(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);
    asm_out_b(sb16_data->port + SB16_PORT_RESET, 0x01);
    sleep_wait(100);
    asm_out_b(sb16_data->port + SB16_PORT_RESET, 0x0);
}

/*
 * speaker on
 */
void sb16_speaker_on(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, SB16_COMMAND_SPEAKER_ON);
}

/*
 * speaker off
 */
void sb16_speaker_off(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, SB16_COMMAND_SPEAKER_OFF);
}

/*
 * upper nibble is left, lower nibble is right, volume is b/t 0-F
 */
void sb16_set_master_volume(struct device* dev, uint8_t v) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);
    asm_out_b(sb16_data->port + SB16_PORT_MIXER, SB16_COMMAND_MASTER_VOLUME);
    asm_out_b(sb16_data->port + SB16_PORT_MIXER_DATA, v);
}

/*
 * time constant
 */
void sb16_set_time_constant(struct device* dev, uint32_t samplerate, uint8_t channels) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);

    // calc time constant
    uint16_t constant = 256 - (1000000 / (channels * samplerate));
    kprintf("   Time Constant %llu\n", constant);

    // set time constant
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, SB16_COMMAND_SET_TIME_CONSTANT);
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, constant);
}

/*
 * start a transfer
 * 24h 8-bit PCM input
 * 14h 8-bit PCM output
 * 75h 8-bit to 4-bit ADPCM output with reference byte
 * 77h 8-bit to 3-bit ADPCM output with reference byte
 * 17h 8-bit to 2-bit ADPCM output with reference byte
 */

void sb16_start_transfer_size(struct device* dev, uint16_t size, uint8_t command, uint8_t mode) {
    ASSERT_NOT_NULL(dev);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);

    // command
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, command);

    // mode
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, mode);

    // calc
    uint8_t low = LOW_OF_WORD(size);
    uint8_t high = HIGH_OF_WORD(size);

    kprintf("   Transfer size %#llX %#llX\n", high, low);
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, low);
    asm_out_b(sb16_data->port + SB16_PORT_WRITE, high);
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
void sb16_play(struct device* dev, uint8_t* buffer, uint16_t rate, uint8_t depth, uint8_t channels, uint64_t len) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(buffer);
    struct sb16_devicedata* sb16_data = (struct sb16_devicedata*)dev->deviceData;
    ASSERT_NOT_NULL(sb16_data);

    kprintf("Incoming buffer %#llX\n", buffer);
    kprintf("Channels %llu\n", channels);
    kprintf("Sample rate %llu\n", rate);
    kprintf("Sample depth %llu\n", depth);
    kprintf("PCM data size %llu\n", len);

    debug_show_memblock(buffer, 16);

    uint32_t chunks = len / ISA_DMA_BUFFER_SIZE;
    kprintf("Chunks: %#llX data len: %#llX dma size: %#llX\n", chunks, len, ISA_DMA_BUFFER_SIZE);

    uint8_t* dma_block_address = (uint8_t*)isadma_get_dma_block(1, ISA_DMA_BUFFER_SIZE);
    kprintf("DMA block for SB16 dma %#llX\n", dma_block_address);

    memcpy(dma_block_address, buffer, ISA_DMA_BUFFER_SIZE);
    kprintf("DMA buffer %#llX\n", dma_block_address);
    debug_show_memblock(dma_block_address, 16);

    sb16_reset(dev);
    sb16_speaker_on(dev);
    sb16_set_master_volume(dev, 0xff);

    if (depth == 8) {
        isadma_init_dma_read(SB16_DMA_8BIT, ISA_DMA_BUFFER_SIZE);
    } else if (depth == 16) {
        isadma_init_dma_read(SB16_DMA_16BIT, ISA_DMA_BUFFER_SIZE);
    } else {
        panic("SB16 unknown bit depth");
    }
    // v4 code, for QEMU
    if (sb16_data->dsp_version == 4) {
        // stereo
        if (channels == 2) {
            sb16_set_stereo_output(dev);
        }
        // time constant
        sb16_set_time_constant(dev, rate, channels);
        // transfer size

        if (depth == 8) {
            /*
             * C0 Program 8-bit DMA mode digitized sound I/O
             * 0x00 mono unsigned
             */
            sb16_start_transfer_size(dev, ISA_DMA_BUFFER_SIZE - 1, 0xC0, 0x00);
        } else if (depth == 16) {
            /*
             * 0xB6 Program 16-bit DMA mode digitized sound I/O
             * 0x30 16 bit stereo unsigned
             */
            sb16_start_transfer_size(dev, ISA_DMA_BUFFER_SIZE - 1, 0xB6, 0x30);
        }
    } else {
        panic("Unknown SB DSP");
    }

    // set time constant
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, SB16_COMMAND_SET_TIME_CONSTANT);

    // 256 - (1000000 / (SampleChannels * SampleRate))
    // for 10898 hz we get 256 - (1000000/10989)=165
    // for 44100 hz we get 256 - (1000000/44100)=234
    // 44100 Hz
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 234);

    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0x41); // output
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0xac); // 0xAC44 (44100)
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0x44);

    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0xc0); // 8 bit output
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0x00); // 8 bit mono unsigned output

    // 8 bit sound
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, SB16_TRANSFER_8BIT);

    // mono and unsigned sound data
    // http://archive.gamedev.net/archive/reference/articles/article443.html
    // TODO should this be 0x14
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, 0x14);

    // COUNT LOW BYTE - COUNT LENGTH-1
    //	kprintf("Low %#llX\n",LOW_OF_W(ISA_DMA_BUFFER_SIZE-1));
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, LOW_OF_W(ISA_DMA_BUFFER_SIZE-1));

    // COUNT HIGH BYTE - COUNT LENGTH-1
    //	kprintf("High %#llX\n",HIGH_OF_W(ISA_DMA_BUFFER_SIZE-1));
    //	asm_out_b(sb16_data->port+SB16_PORT_WRITE, HIGH_OF_W(ISA_DMA_BUFFER_SIZE-1));
}

void sb16_devicemgr_register_device(uint64_t port) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "Soundblaster 16");
    deviceinstance->devicetype = DSP;
    deviceinstance->init = &sb16_device_init;
    /*
     * device api
     */
    struct deviceapi_dsp* api = (struct deviceapi_dsp*)kmalloc(sizeof(struct deviceapi_dsp));
    api->play = sb16_play;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct sb16_devicedata* deviceData = (struct sb16_devicedata*)kmalloc(sizeof(struct sb16_devicedata));
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
bool sb16_detect(uint32_t port) {
    asm_out_b(port + SB16_PORT_RESET, 0x01);
    asm_out_b(port + SB16_PORT_RESET, 0x00);
    uint8_t x = 0x00;
    do {
        x = asm_in_b(port + SB16_PORT_READ_STATUS);
    } while (0 == (x & 0x80));
    x = asm_in_b(port + SB16_PORT_READ);
    if (x == 0xaa) {
        return true;
    }
    return false;
}

void sb16_devicemgr_register_devices() {
    for (uint8_t i = 0; i < 4; i++) {
        if (sb16_detect(SB_BASE_PORTS[i])) {
            sb16_devicemgr_register_device(SB_BASE_PORTS[i]);
        }
    }
}
