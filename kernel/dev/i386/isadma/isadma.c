//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

//#define ISADMA_DEBUG

#include <dev/i386/isadma/isadma.h>
#include <dev/i386/isadma/isadma_page.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kprintf/kprintf.h>

// https://wiki.osdev.org/DMA
// http://www.osdever.net/documents/dmaprogramming.pdf

// channels 0-3 (8 bit)
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_0_4 0x00
#define ISA_DMA_CHAN03_COUNT_REGISTER_0_4 0x01
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_1_5 0x02
#define ISA_DMA_CHAN03_COUNT_REGISTER_1_5 0x03
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_2_6 0x04
#define ISA_DMA_CHAN03_COUNT_REGISTER_2_6 0x05
#define ISA_DMA_CHAN03_START_ADDRESS_REGISTER_3_7 0x06
#define ISA_DMA_CHAN03_COUNT_REGISTER_3_7 0x07
#define ISA_DMA_CHAN03_STATUS_REGISTER 0x08
#define ISA_DMA_CHAN03_COMMAND_REGISTER 0x08
#define ISA_DMA_CHAN03_REQUEST_REGISTER 0x09
#define ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER 0x0A
#define ISA_DMA_CHAN03_MODE_REGISTER 0x0B
#define ISA_DMA_CHAN03_FLIP_FLOP_RESET_REGISTER 0x0C
#define ISA_DMA_CHAN03_INTERMEDIATE_REGISTER 0x0D
#define ISA_DMA_CHAN03_MASTER_RESET_REGISTER 0x0D
#define ISA_DMA_CHAN03_MASK_RESET_REGISTER 0x0E
#define ISA_DMA_CHAN03__MULTICHANNEL_MASK_RESET_REGISTER 0x0F

// channels 4-7 (16 bit)
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_0_4 0xC0
#define ISA_DMA_CHAN47_COUNT_REGISTER_0_4 0xC2
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_1_5 0xC4
#define ISA_DMA_CHAN47_COUNT_REGISTER_1_5 0xC6
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_2_6 0xC8
#define ISA_DMA_CHAN47_COUNT_REGISTER_2_6 0xCA
#define ISA_DMA_CHAN47_START_ADDRESS_REGISTER_3_7 0xCC
#define ISA_DMA_CHAN47_COUNT_REGISTER_3_7 0xCE
#define ISA_DMA_CHAN47_STATUS_REGISTER 0xD0
#define ISA_DMA_CHAN47_COMMAND_REGISTER 0xD0
#define ISA_DMA_CHAN47_REQUEST_REGISTER 0xD2
#define ISA_DMA_CHAN47_SINGLE_CHANNEL_MASK_REGISTER 0xD4
#define ISA_DMA_CHAN47_MODE_REGISTER 0xD6
#define ISA_DMA_CHAN47_FLIP_FLOP_RESET_REGISTER 0xD8
#define ISA_DMA_CHAN47_INTERMEDIATE_REGISTER 0xDA
#define ISA_DMA_CHAN47_MASTER_RESET_REGISTER 0xDA
#define ISA_DMA_CHAN47_MASK_RESET_REGISTER 0xDC
#define ISA_DMA_CHAN47__MULTICHANNEL_MASK_RESET_REGISTER 0xDE

// page address registers
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_0 0x87
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_1 0x83
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_2 0x81
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_3 0x82
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_4 0x8F
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_5 0x8B
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_6 0x89
#define ISA_DMA_CHANNEL_PAGE_ADDRESS_7 0x8A

// mode selection bits 7:6
#define ISA_DMA_DEMAND_MODE 0x00           // 00
#define ISA_DMA_SINGLE_MODE 0x40           // 01
#define ISA_DMA_BLOCK_MODE 								` 0x80  // 10
#define ISA_DMA_CASCADE_MODE 0xC0          // 11

// address increment / decrement bit 5
#define ISA_DMA_ADDRESS_DECREMENT 0x20  // 1
#define ISA_DMA_ADDRESS_INCREMENT 0x00  // 0

// autoInitialization enable bit 4
#define ISA_DMA_AUTO_INIT 0x10     // 1
#define ISA_DMA_SINGLE_CYCLE 0x00  // 0

// transfer type bits 3:2
#define ISA_DMA_VERIFY_TRANSFER 0x00  // 00
#define ISA_DMA_WRITE_TRANSFER 0x04   // 01
#define ISA_DMA_READ_TRANSFER 0x08    // 10

// channel, bits 1:0
#define ISA_DMA_CHANNEL_0_4 0x00  // 00
#define ISA_DMA_CHANNEL_1_5 0x01  // 01
#define ISA_DMA_CHANNEL_2_6 0x02  // 10
#define ISA_DMA_CHANNEL_3_7 0x03  // 11

/*
 * this variable is set up by the MM
 */
void* isadma_buf = 0;

/*
 * parameters for a channel
 */
struct isa_dma_channel_parameters {
    uint32_t DMAAddressPort;
    uint32_t DMACountPort;
    uint32_t DMAPagePort;
    uint32_t DMAMaskReg;
    uint32_t DMAClearReg;
    uint32_t DMAModeReg;
    bool eightbit;
    uint8_t DMAChannelFlags;  // bits 0:1 in channel mode registers

    /*
     * block assigned by the MM
     */
    uint64_t DMABlock;
    /*
     * DMA channel
     */
    uint8_t channel;
};

void isadma_show_dma_parameters(struct isa_dma_channel_parameters* parameters) {
    kprintf("   DMA channel %#X, IO Block %#X\n", parameters->channel, parameters->DMABlock);
    uint16_t page = isadma_address_to_page(parameters->DMABlock);
    uint16_t buffer = isadma_address_to_buffer(parameters->DMABlock);
    kprintf("   DMA page %#X, buffer %#X\n", page, buffer);
    kprintf("   DMAAddressPort %#X\n", parameters->DMAAddressPort);
    kprintf("   DMACountPort %#X\n", parameters->DMACountPort);
    kprintf("   DMAPagePort %#X\n", parameters->DMAPagePort);
    kprintf("   DMAMaskReg %#X\n", parameters->DMAMaskReg);
    kprintf("   DMAClearReg %#X\n", parameters->DMAClearReg);
    kprintf("   DMAModeReg %#X\n", parameters->DMAModeReg);
    kprintf("   DMAChannelFlags %#X\n", parameters->DMAChannelFlags);
    kprintf("   8-bit %#X\n", parameters->eightbit);
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
void getDMAParameters(uint8_t channel, struct isa_dma_channel_parameters* parameters) {
    ASSERT(channel >= 0);
    ASSERT(channel < ISA_DMA_NUM_BUFFERS);
    ASSERT_NOT_NULL(parameters);

    parameters->channel = channel;
    switch (channel) {
        case 0:
            parameters->DMAAddressPort = ISA_DMA_CHAN03_START_ADDRESS_REGISTER_0_4;
            parameters->DMACountPort = ISA_DMA_CHAN03_COUNT_REGISTER_0_4;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_0;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_0_4;
            break;
        case 1:
            parameters->DMAAddressPort = ISA_DMA_CHAN03_START_ADDRESS_REGISTER_1_5;
            parameters->DMACountPort = ISA_DMA_CHAN03_COUNT_REGISTER_1_5;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_1;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_1_5;
            break;
        case 2:
            parameters->DMAAddressPort = ISA_DMA_CHAN03_START_ADDRESS_REGISTER_2_6;
            parameters->DMACountPort = ISA_DMA_CHAN03_COUNT_REGISTER_2_6;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_2;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_2_6;
            break;
        case 3:
            parameters->DMAAddressPort = ISA_DMA_CHAN03_START_ADDRESS_REGISTER_3_7;
            parameters->DMACountPort = ISA_DMA_CHAN03_COUNT_REGISTER_3_7;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_3;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_3_7;
            break;
        case 4:
            parameters->DMAAddressPort = ISA_DMA_CHAN47_START_ADDRESS_REGISTER_0_4;
            parameters->DMACountPort = ISA_DMA_CHAN47_COUNT_REGISTER_0_4;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_4;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_0_4;
            break;
        case 5:
            parameters->DMAAddressPort = ISA_DMA_CHAN47_START_ADDRESS_REGISTER_1_5;
            parameters->DMACountPort = ISA_DMA_CHAN47_COUNT_REGISTER_1_5;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_5;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_1_5;
            break;
        case 6:
            parameters->DMAAddressPort = ISA_DMA_CHAN47_START_ADDRESS_REGISTER_2_6;
            parameters->DMACountPort = ISA_DMA_CHAN47_COUNT_REGISTER_2_6;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_6;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_2_6;
            break;
        case 7:
            parameters->DMAAddressPort = ISA_DMA_CHAN47_START_ADDRESS_REGISTER_3_7;
            parameters->DMACountPort = ISA_DMA_CHAN47_COUNT_REGISTER_3_7;
            parameters->DMAPagePort = ISA_DMA_CHANNEL_PAGE_ADDRESS_7;
            parameters->DMAChannelFlags = ISA_DMA_CHANNEL_3_7;
            break;
    }
    if (channel < 4) {
        parameters->DMAMaskReg = ISA_DMA_CHAN03_SINGLE_CHANNEL_MASK_REGISTER;
        parameters->DMAClearReg = ISA_DMA_CHAN03_FLIP_FLOP_RESET_REGISTER;
        parameters->DMAModeReg = ISA_DMA_CHAN03_MODE_REGISTER;
        parameters->eightbit = 1;
    } else {
        parameters->DMAMaskReg = ISA_DMA_CHAN47_SINGLE_CHANNEL_MASK_REGISTER;
        parameters->DMAClearReg = ISA_DMA_CHAN47_FLIP_FLOP_RESET_REGISTER;
        parameters->DMAModeReg = ISA_DMA_CHAN47_MODE_REGISTER;
        parameters->eightbit = 0;
    }
    /*
     * DMA memory block
     */
    parameters->DMABlock = isadma_get_dma_block(channel, ISA_DMA_BUFFER_SIZE);
}

/*
 * reset the flip flop
 */
void isadma_reset_flipflop(struct isa_dma_channel_parameters* channel_parameters) {
    asm_out_b(channel_parameters->DMAClearReg, 0xFF);
}

/*
 * set byte count
 */
void isadma_set_transfer_size(struct isa_dma_channel_parameters* channel_parameters, uint16_t len) {
    isadma_reset_flipflop(channel_parameters);
    uint8_t count_low = LOW_OF_W(len - 1);
    uint8_t count_high = HIGH_OF_W(len - 1);
    kprintf("   Count %#X %#X\n", count_high, count_low);

    asm_out_b(channel_parameters->DMACountPort, count_low);
    asm_out_b(channel_parameters->DMACountPort, count_high);
}

/*
 * set address of buffer
 */
void isadma_set_buffer_address(struct isa_dma_channel_parameters* channel_parameters) {
    isadma_reset_flipflop(channel_parameters);
    uint16_t page = isadma_address_to_page(channel_parameters->DMABlock);
    uint16_t buffer = isadma_address_to_buffer(channel_parameters->DMABlock);

    // page
    asm_out_b(channel_parameters->DMAPagePort, page);
    kprintf("   Page %#X\n", page);

    // buffer
    uint8_t position_low = LOW_OF_W(buffer);
    uint8_t position_high = HIGH_OF_W(buffer);
    kprintf("   Position %#X %#X\n", position_high, position_low);

    asm_out_b(channel_parameters->DMAAddressPort, position_low);
    asm_out_b(channel_parameters->DMAAddressPort, position_high);
}

/*
 * mask channel (the +0x04 is b/c bit 2 of the register enables masking)
 */
void isadma_mask_channel(struct isa_dma_channel_parameters* channel_parameters) {
    asm_out_b(channel_parameters->DMAMaskReg, channel_parameters->channel + 0x04);
}

/*
 * unmask channel
 */
void isadma_unmask_channel(struct isa_dma_channel_parameters* channel_parameters) {
    asm_out_b(channel_parameters->DMAMaskReg, channel_parameters->channel);
}

/**
 * transfer mode
 */
void isadma_set_transfer_mode(struct isa_dma_channel_parameters* channel_parameters, uint8_t mode) {
    asm_out_b(channel_parameters->DMAModeReg, mode);
    kprintf("   Transfer Mode %#X\n", mode);
}

/*
 * this function shared by read and write
 */
void isadma_init_dma(uint8_t channel, uint32_t len, uint8_t rw_mode) {
    ASSERT(channel >= 0);
    ASSERT(channel < ISA_DMA_NUM_BUFFERS);
    ASSERT(channel != 0);
    ASSERT(channel != 4);

    kprintf("DMA init for channel %#X with len %#X\n", channel, len);

    /*
     * get the channel parameters
     */
    struct isa_dma_channel_parameters channel_parameters;
    getDMAParameters(channel, &channel_parameters);
    isadma_show_dma_parameters(&channel_parameters);

    // mask channel
    isadma_mask_channel(&channel_parameters);

    // reset flip-flop
    isadma_reset_flipflop(&channel_parameters);

    // transfer mode
    uint8_t mode = ISA_DMA_SINGLE_MODE | ISA_DMA_ADDRESS_INCREMENT | ISA_DMA_SINGLE_CYCLE | rw_mode | channel_parameters.DMAChannelFlags;
    isadma_set_transfer_mode(&channel_parameters, mode);

    // buffer address
    isadma_set_buffer_address(&channel_parameters);

    // transfer size
    isadma_set_transfer_size(&channel_parameters, len);

    // enable channel 1
    isadma_unmask_channel(&channel_parameters);
}

void isadma_init_dma_read(uint8_t channel, uint32_t len) {
    isadma_init_dma(channel, len, ISA_DMA_READ_TRANSFER);
}

void isadma_init_dma_write(uint8_t channel, uint32_t len) {
    isadma_init_dma(channel, len, ISA_DMA_WRITE_TRANSFER);
}
/*
 * get the DMA address for a channel
 */
uint64_t isadma_get_dma_block(uint8_t channel, uint32_t len) {
    ASSERT(channel >= 0);
    ASSERT(channel < ISA_DMA_NUM_BUFFERS);
    if (len > ISA_DMA_BUFFER_SIZE) {
        panic("buffer too large for DMA");
    }

    uint64_t start = (uint64_t)((uint64_t)isadma_buf + (ISA_DMA_BUFFER_SIZE * channel));
    uint64_t end = start + ISA_DMA_BUFFER_SIZE - 1;

    if (isadma_address_to_page(start) != isadma_address_to_page(end)) {
        panic("DMA buffer crosses page boundary");
    }
    return start;
}

/*
 * perform device instance specific init here
 */
void isadma_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    isadma_buf = iobuffers_request_buffer(ISA_DMA_BUFSIZ);

    ASSERT_NOT_NULL(isadma_buf);

    kprintf("Init %s (%s)\n", dev->description, dev->name);
    /*
     * show DMA mem
     */
    uint64_t end_dma_area = (uint64_t)isadma_buf + ISA_DMA_BUFSIZ;
    ASSERT((end_dma_area <= ISA_DMA_64M));
    kprintf("   DMA area is %#X-%#X\n", isadma_buf, end_dma_area - 1);
    /*
     * show DMA parameters
     */
#ifdef ISADMA_DEBUG
    for (uint8_t i = 0; i < 8; i++) {
        struct isa_dma_channel_parameters channel_parameters;
        getDMAParameters(i, &channel_parameters);
        kprintf("\n");
        isadma_show_dma_parameters(&channel_parameters);
    }
#endif
}

void isadma_device_uninit(struct device* dev) {
    iobuffers_release_buffer(isadma_buf);
}

void isadma_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "8237 ISA DMA");
    deviceinstance->devicetype = DMA;
    deviceinstance->init = &isadma_device_init;
    deviceinstance->uninit = &isadma_device_uninit;
    devicemgr_register_device(deviceinstance);
}
