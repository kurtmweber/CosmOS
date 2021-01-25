//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * NE2000 support based on code from here https://github.com/matijaspanic/NE2000/blob/master/NE2000.c
 */

#include <dev/i386/network/ne2000isa/ne2000isa.h>
#include <sys/asm/asm.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <types.h>

#define NE2000ISA_BASE_ADDRESS 0x300
#define NE2000ISA_IRQ 9

// REGISTERS
#define CR NE2000ISA_BASE_ADDRESS + 0x00       // Command Register
#define RDMA NE2000ISA_BASE_ADDRESS + 0x10     // Remote DMA Port
#define RSTPORT NE2000ISA_BASE_ADDRESS + 0x18  // Reset Port

// page 0
#define PSTART NE2000ISA_BASE_ADDRESS + 0x01  // Page Start Register (W) - read in page2
#define PSTOP NE2000ISA_BASE_ADDRESS + 0x02   // Page Stop Register (W) - read in page 2
#define BNRY NE2000ISA_BASE_ADDRESS + 0x03    // Boundary Register (R/W)
#define TSR NE2000ISA_BASE_ADDRESS + 0x04     // Transmit Status Register (R)
#define TPSR NE2000ISA_BASE_ADDRESS + 0x04    // Transmit Page Start Register (W) - read in page 2
#define TBCR0 NE2000ISA_BASE_ADDRESS + 0x05   // Transmit Byte Count Registers (W)
#define TBCR1 NE2000ISA_BASE_ADDRESS + 0x06   // Transmit Byte Count Registers (W)
#define ISR NE2000ISA_BASE_ADDRESS + 0x07     // Interrupt Status Register (R/W)
#define CRDA0 NE2000ISA_BASE_ADDRESS + 0x08   // Current Remote DMA Address registers (R)
#define CRDAl NE2000ISA_BASE_ADDRESS + 0x09   // Current Remote DMA Address registers (R)
#define RSAR0 NE2000ISA_BASE_ADDRESS + 0x08   // Remote Start Address Registers (W)
#define RSAR1 NE2000ISA_BASE_ADDRESS + 0x09   // Remote Start Address Registers (W)
#define RBCR0 NE2000ISA_BASE_ADDRESS + 0x0A   // Remote Byte Count Registers (W)
#define RBCR1 NE2000ISA_BASE_ADDRESS + 0x0B   // Remote Byte Count Registers (W)
#define RSR NE2000ISA_BASE_ADDRESS + 0x0C     // Receive Status Register (R)
#define RCR NE2000ISA_BASE_ADDRESS + 0x0C     // Receive Configuration Register (W) - read in page 2
#define CNTR0 NE2000ISA_BASE_ADDRESS + 0x0D   // Frame Alignment Error Tally Counter Register (R)
#define TCR NE2000ISA_BASE_ADDRESS + 0x0D     // Transmit Configuration Register (W) - read in page 2
#define CNTR1 NE2000ISA_BASE_ADDRESS + 0x0E   // CRC Error Tally Counter Register (R)
#define DCR NE2000ISA_BASE_ADDRESS + 0x0E     // Data Configuration Register (W) - read in page 2
#define CNTR2 NE2000ISA_BASE_ADDRESS + 0x0F   // Missed Packet Tally Counter Register (R)
#define IMR NE2000ISA_BASE_ADDRESS + 0x0F     // Interrupt Mask Register (W) - read in page 2

// page 1
#define PAR0 NE2000ISA_BASE_ADDRESS + 0x01  // Physical Address Registers (R/W)
#define PAR1 NE2000ISA_BASE_ADDRESS + 0x02
#define PAR2 NE2000ISA_BASE_ADDRESS + 0x03
#define PAR3 NE2000ISA_BASE_ADDRESS + 0x04
#define PAR4 NE2000ISA_BASE_ADDRESS + 0x05
#define PAR5 NE2000ISA_BASE_ADDRESS + 0x06
#define CURR                                                                                                           \
    NE2000ISA_BASE_ADDRESS +                                                                                           \
        0x07  // Current Page Register (R/W)                                                      \
        // This register points to the page address of the first receive buffer page to be used for a packet reception.
#define MAR0 NE2000ISA_BASE_ADDRESS + 0x08  // Multicast Register 0
#define MAR1 NE2000ISA_BASE_ADDRESS + 0x09  // Multicast Register 1
#define MAR2 NE2000ISA_BASE_ADDRESS + 0x0A  // Multicast Register 2
#define MAR3 NE2000ISA_BASE_ADDRESS + 0x0B  // Multicast Register 3
#define MAR4 NE2000ISA_BASE_ADDRESS + 0x0C  // Multicast Register 4
#define MAR5 NE2000ISA_BASE_ADDRESS + 0x0D  // Multicast Register 5
#define MAR6 NE2000ISA_BASE_ADDRESS + 0x0E  // Multicast Register 6
#define MAR7 NE2000ISA_BASE_ADDRESS + 0x0F  // Multicast Register 7

// COMMAND REGISTER BITS
#define CR_STOP 0x01      // Stop (Reset) NIC
#define CR_START 0x02     // Start NIC
#define CR_TRANSMIT 0x04  // Must be 1 to transmit packet
// only one of next four can be used at the same time
#define CR_DMAREAD 0x08     // remote DMA read
#define CR_DMAWRITE 0x10    // remote DMA write */
#define CR_SENDPACKET 0x18  // send packet
#define CR_NODMA 0x20       /* abort/complete remote DMA */
//
#define CR_PAGE0 0x00 /* select register page 0 */
#define CR_PAGE1 0x40 /* select register page 1 */
#define CR_PAGE2 0x80 /* select register page 2 */

// ISR Register Bits
#define ISR_PRX 0x01  // indicates packet received with no errors
#define ISR_PTX 0x02  // indicates packet transmitted with no error
#define ISR_RXE                                                                                                        \
    0x04  // set when a packet received with one or more of the following errors: CRC error, Frame alignment error, Missed packet
#define ISR_TXE 0x08  // set when a packet transmission is aborted due to excessive collisions
#define ISR_OVW 0x10  // set when the receive buffer has been exhausted
#define ISR_CNT 0x20  // Set when MSB of one or more of the network tally counters has been set
#define ISR_RDC 0x40  // Set when remote DMA operation has been completed
#define ISR_RST                                                                                                        \
    0x80  // set when NIC enters reset state and is cleared when a start command is issued to the CR                   \
        // It is also set when receive buffer overflows and is cleared when one or more packets have been read from the buffer

/* For the Data Control Register */
#define DCR_BYTEDMA 0x00
#define DCR_WORDDMA 0x01
#define DCR_NOLPBK 0x08
#define DCR_ARM 0x10
#define DCR_FIFO2 0x00
#define DCR_FIFO4 0x20
#define DCR_FIFO8 0x40
#define DCR_FIFO12 0x60

/* For the Receive Control Register */
#define RCR_AB 0x04
#define RCR_AM 0x08
#define RCR_PROMISCUOUS 0x10
#define RCR_MONITOR 0x20

/* For the Transmission Control Register */
#define TCR_NOLPBK 0x00
#define TCR_INTLPBK 0x02
#define TCR_EXTLPBK 0x04
#define TCR_EXTLPBK2 0x06

#define TXSTART 0x40  // Start of TX buffers
#define RXSTART 0x46  // Start of RX buffers
#define RXSTOP 0x60   // End of RX buffers

#define MTU_SIZE 600

/*
 * MAC address
 */
uint8_t net_mac_isa[6];

void ne2000isa_init(void);

void ne2000isa_irq_handler(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("%");
}
/*
 * perform device instance specific init here
 */
uint8_t ne2000_isa_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    interrupt_router_register_interrupt_handler(NE2000ISA_IRQ, &ne2000isa_irq_handler);
    kprintf("Init %s at IRQ %llu (%s)\n", dev->description, NE2000ISA_IRQ, dev->name);
    // do the init
    ne2000isa_init();
    return 1;
}

void ne2000isa_ethernet_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    panic("Ethernet read not implemented yet");
}
void ne2000isa_ethernet_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    panic("Ethernet write not implemented yet");
}

/**
 * find all NE2000 devices and register them
 */
void ne2000isa_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &ne2000_isa_init;
    deviceinstance->devicetype = NIC;
    devicemgr_set_device_description(deviceinstance, "NE2000 ISA");
    /*
     * the device api
     */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*)kmalloc(sizeof(struct deviceapi_ethernet));
    api->write = &ne2000isa_ethernet_read;
    api->read = &ne2000isa_ethernet_write;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

void ne2000isa_init() {
    asm_out_b(CR, (CR_PAGE0 | CR_NODMA | CR_STOP));      // set page 0, turn off DMA, tell the NIC to stop
    sleep_wait(10);                                      // wait for traffic to complete
    asm_out_b(DCR, DCR_BYTEDMA | DCR_NOLPBK | DCR_ARM);  // we want byte-wide DMA, automatic DMA transfers,
    asm_out_b(RBCR0, 0x00);                              // byte count zero
    asm_out_b(RBCR1, 0x00);                              // byte count zero
    asm_out_b(RCR, RCR_AB);                              // receive configuration of (Accept Broadcast)
    asm_out_b(TCR, TCR_INTLPBK);                         // set internal lookback

    asm_out_b(TPSR, TXSTART);    // set the start page address of the packet to the transmitted.
    asm_out_b(PSTART, RXSTART);  // set the start page address of the receive buffer ring
    asm_out_b(BNRY, RXSTART);    // set Boundary Register
    asm_out_b(PSTOP, RXSTOP);    // set the stop page address of the receive buffer ring

    // get mac
    asm_out_b(CR, (CR_PAGE1 | CR_NODMA | CR_STOP));
    net_mac_isa[0] = asm_in_b(PAR0);
    net_mac_isa[1] = asm_in_b(PAR1);
    net_mac_isa[2] = asm_in_b(PAR2);
    net_mac_isa[3] = asm_in_b(PAR3);
    net_mac_isa[4] = asm_in_b(PAR4);
    net_mac_isa[5] = asm_in_b(PAR5);

    kprintf("   MAC %#hX:%#hX:%#hX:%#hX:%#hX:%#hX\n", net_mac_isa[0], net_mac_isa[1], net_mac_isa[2], net_mac_isa[3],
            net_mac_isa[4], net_mac_isa[5]);

    asm_out_b(CR, (CR_PAGE0 | CR_NODMA | CR_STOP));
    asm_out_b(DCR, DCR_FIFO8 | DCR_NOLPBK | DCR_ARM);
    asm_out_b(CR, (CR_NODMA | CR_START));
    asm_out_b(ISR, 0xFF);  // clear all interrupts
    asm_out_b(IMR, 0xFF);  // all interupts
    asm_out_b(TCR, 0x00);  // normal operation

    asm_out_b(CURR, RXSTART);              // init curr pointer
    asm_out_b(CR, (CR_NODMA | CR_START));  // start the NIC
}

void ne2000isa_send(uint8_t* packet, uint16_t length) {
    uint16_t i;

    if (length < 0x40) {
        length = 0x40;
    }

    i = 0;
    // If there is still a packet in transmission, wait until it's done!
    while ((i < 50000) && (asm_in_b(CR) & CR_TRANSMIT)) {
        i++;
    }

    // Abort any currently running "DMA" operations
    asm_out_b(CR, CR_PAGE0 | CR_START | CR_NODMA);
    asm_out_b(RBCR0, length & 0xff);
    asm_out_b(RBCR1, length >> 8);
    asm_out_b(RSAR0, 0x00);
    asm_out_b(RSAR1, TXSTART);
    asm_out_b(CR, CR_PAGE0 | CR_START | CR_DMAWRITE);
    for (i = 0; i < length; i++) {
        asm_out_b(RDMA, packet[i]);
    }

    // Wait for something here?
    asm_out_b(CR, CR_PAGE0 | CR_START | CR_NODMA);
    asm_out_b(TBCR0, length & 0xff);
    asm_out_b(TBCR1, length >> 8);
    asm_out_b(TPSR, TXSTART);
    asm_out_b(CR, CR_PAGE0 | CR_START | CR_TRANSMIT);
}

uint16_t ne2000isa_recieve(uint8_t* packet, uint16_t max_size) {
    uint8_t curr, bnry;
    uint16_t i = 0;

    uint16_t net_recvdbytes;

    asm_out_b(CR, CR_PAGE1 | CR_START | CR_NODMA);  // goto register page 1
    curr = asm_in_b(CURR);                          // read the CURRent pointer

    asm_out_b(CR, CR_PAGE0 | CR_START | CR_NODMA);  // goto register page 0

    // read the boundary register pointing to the beginning of the packet
    bnry = asm_in_b(BNRY);

    if (bnry == curr)
        return 0;

    // if boundary pointer is invalid
    if ((bnry >= RXSTOP) || (bnry < RXSTART)) {
        // reset the contents of the buffer and exit
        asm_out_b(BNRY, RXSTART);
        asm_out_b(CR, (CR_PAGE1 | CR_NODMA | CR_START));
        asm_out_b(CURR, RXSTART);
        asm_out_b(CR, (CR_NODMA | CR_START));
        return 0;
    }

    if (bnry > RXSTOP - 1) {
        bnry = RXSTART;
    }

    asm_out_b(RBCR0, 0xff);
    asm_out_b(RBCR1, 0xff);
    asm_out_b(RSAR0, 0);     // low byte of start address (0)
    asm_out_b(RSAR1, bnry);  // high byte of start address (BNRY)

    // begin the dma read
    asm_out_b(CR, CR_PAGE0 | CR_START | CR_DMAREAD);

    asm_in_b(RDMA);

    bnry = asm_in_b(RDMA);  // next-pointer
    if (bnry < RXSTART) {
        bnry = RXSTOP;
    };

    net_recvdbytes = asm_in_b(RDMA);
    net_recvdbytes += (asm_in_b(RDMA) << 8);

    if (net_recvdbytes > MTU_SIZE) {
        net_recvdbytes = MTU_SIZE;
    }

    for (i = 0; i < net_recvdbytes; i++) {
        packet[i] = asm_in_b(RDMA);
    }

    asm_out_b(CR, CR_PAGE0 | CR_START | CR_NODMA);  // set page 0
    asm_out_b(BNRY, bnry);                          // write updated bnry pointer

    return net_recvdbytes;
}
