//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* NE2000 support based on code from here https://github.com/matijaspanic/NE2000/blob/master/NE2000.c
*/

#include <dev/network/ne2000/ne2000.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <dev/pci/pci.h>
#include <types.h>
#include <asm/io.h>

// REGISTERS
#define CR				0x00 // Command Register
#define RDMA			0x10 // Remote DMA Port
#define RSTPORT 		0x18 // Reset Port
					
// page 0
#define PSTART			0x01 // Page Start Register (W) - read in page2
#define PSTOP			0x02 // Page Stop Register (W) - read in page 2
#define BNRY			0x03 // Boundary Register (R/W)
#define TSR 			0x04 // Transmit Status Register (R)
#define TPSR			0x04 // Transmit Page Start Register (W) - read in page 2
#define TBCR0			0x05 // Transmit Byte Count Registers (W)
#define TBCR1			0x06 // Transmit Byte Count Registers (W)
#define ISR 			0x07 // Interrupt Status Register (R/W)
#define CRDA0			0x08 // Current Remote DMA Address registers (R)
#define CRDAl			0x09 // Current Remote DMA Address registers (R)
#define RSAR0			0x08 // Remote Start Address Registers (W)
#define RSAR1			0x09 // Remote Start Address Registers (W)
#define RBCR0			0x0A // Remote Byte Count Registers (W)
#define RBCR1			0x0B // Remote Byte Count Registers (W)
#define RSR 			0x0C // Receive Status Register (R)
#define RCR 			0x0C // Receive Configuration Register (W) - read in page 2
#define CNTR0			0x0D // Frame Alignment Error Tally Counter Register (R)
#define TCR 			0x0D // Transmit Configuration Register (W) - read in page 2
#define CNTR1			0x0E // CRC Error Tally Counter Register (R)
#define DCR 			0x0E // Data Configuration Register (W) - read in page 2
#define CNTR2			0x0F // Missed Packet Tally Counter Register (R)
#define IMR 			0x0F // Interrupt Mask Register (W) - read in page 2

// page 1				
#define PAR0			0x01 // Physical Address Registers (R/W)
#define PAR1			0x02
#define PAR2			0x03
#define PAR3			0x04
#define PAR4			0x05
#define PAR5			0x06
#define CURR			0x07 // Current Page Register (R/W)
							 // This register points to the page address of the first receive buffer page to be used for a packet reception.


// COMMAND REGISTER BITS
#define CR_STOP 		0x01	// Stop (Reset) NIC
#define CR_START		0x02	// Start NIC
#define CR_TRANSMIT 	0x04	// Must be 1 to transmit packet
// only one of next four can be used at the same time
#define CR_DMAREAD		0x08	// remote DMA read
#define CR_DMAWRITE 	0x10	// remote DMA write */
#define CR_SENDPACKET	0x18	// send packet
#define CR_NODMA		0x20	/* abort/complete remote DMA */
// 
#define CR_PAGE0		0x00	/* select register page 0 */
#define CR_PAGE1		0x40	/* select register page 1 */
#define CR_PAGE2		0x80	/* select register page 2 */

// ISR Register Bits
#define ISR_PRX 		0x01	// indicates packet received with no errors   
#define ISR_PTX 		0x02	// indicates packet transmitted with no error
#define ISR_RXE 		0x04	// set when a packet received with one or more of the following errors: CRC error, Frame alignment error, Missed packet 
#define ISR_TXE 		0x08	// set when a packet transmission is aborted due to excessive collisions
#define ISR_OVW 		0x10	// set when the receive buffer has been exhausted
#define ISR_CNT 		0x20	// Set when MSB of one or more of the network tally counters has been set
#define ISR_RDC 		0x40	// Set when remote DMA operation has been completed
#define ISR_RST 		0x80	// set when NIC enters reset state and is cleared when a start command is issued to the CR
								// It is also set when receive buffer overflows and is cleared when one or more packets have been read from the buffer

/* For the Data Control Register */
#define DCR_BYTEDMA 0x00
#define DCR_WORDDMA 0x01
#define DCR_NOLPBK	0x08
#define DCR_ARM 	0x10
#define DCR_FIFO2	0x00
#define DCR_FIFO4	0x20
#define DCR_FIFO8	0x40
#define DCR_FIFO12	0x60

/* For the Receive Control Register */
#define RCR_AB		    0x04
#define RCR_AM		    0x08
#define RCR_PROMISCUOUS 0x10
#define RCR_MONITOR     0x20

/* For the Transmission Control Register */
#define TCR_NOLPBK	    0x00
#define TCR_INTLPBK     0x02
#define TCR_EXTLPBK     0x04
#define TCR_EXTLPBK2	0x06

#define TXSTART 0x40	// Start of TX buffers
#define RXSTART 0x46	// Start of RX buffers
#define RXSTOP	0x60	// End of RX buffers

#define MTU_SIZE 600

/*
* MAC address
*/
uint8_t net_mac[6];

void ne2000_init(void);

void ne2000_irq_handler(stackFrame *frame){
}
/*
* perform device instance specific init here
*/
void NE200Init(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    registerInterruptHandler(pci_dev->irq, &ne2000_irq_handler);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id);
    // do the init
    ne2000_init();
}

void NE2000SearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = newDevice();
    deviceinstance->init =  &NE200Init;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ETHERNET;
    deviceSetDescription(deviceinstance, "NE2000");
    registerDevice(deviceinstance);
}

/**
* find all NE2000 devices and register them
*/
void ne2000_register_devices() {
    pci_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x10EC,0x8029, &NE2000SearchCB);
}

void ne2000_init() {	
	asm_out_b(CR, (CR_PAGE0|CR_NODMA|CR_STOP));     // set page 0
//	_delay_ms(2);					                // wait for traffic to complete
	asm_out_b(DCR, DCR_BYTEDMA|DCR_NOLPBK|DCR_ARM);
	asm_out_b(RBCR0,0x00);
	asm_out_b(RBCR1,0x00);
	asm_out_b(RCR, RCR_AB);
	asm_out_b(TCR, TCR_INTLPBK); 	                // set internal lookback

	asm_out_b(TPSR, TXSTART);			            // set the start page address of the packet to the transmitted.
	asm_out_b(PSTART, RXSTART);		                // set the start page address of the receive buffer ring
	asm_out_b(BNRY, RXSTART);			            // set Boundary Register	
	asm_out_b(PSTOP, RXSTOP);			            // set the stop page address of the receive buffer ring 
		
	asm_out_b(CR, (CR_PAGE0|CR_NODMA|CR_STOP));
	asm_out_b(DCR, DCR_FIFO8|DCR_NOLPBK|DCR_ARM);
	asm_out_b(CR, (CR_NODMA|CR_START));
	asm_out_b(ISR,0xFF); 			                // clear all interrupts
	asm_out_b(IMR, 0x00);			                // no interupts
	asm_out_b(TCR, 0x00);			                // normal operation
	
    // read mac from eeprom
	asm_out_b(RBCR0, 32); // Intend to read 32 Bytes
	asm_out_b(RBCR1, 0);
	asm_out_b(RSAR0, 0); // low byte of start address (0x0000)
	asm_out_b(RSAR1, 0); // high byte of start address
	asm_out_b(CR, CR_PAGE0 | CR_START | CR_DMAREAD);
	// for some reason, 2 reads are required, otherwise you get duplicate
	// values. the comments in the linux driver talk about values being
	// "doubled up", but i don't know why. whatever - it works this way
	// and i don't have time to investigate :)
	for (uint8_t i = 0; i < 6; i++) {
		asm_in_b(RDMA);
		net_mac[i] = asm_in_b(RDMA);
	}
	// end (abort) the DMA transfer
	asm_out_b(CR, CR_PAGE0 | CR_START | CR_NODMA);

	kprintf("MAC  %#hX:%#hX:%#hX:%#hX:%#hX:%#hX\n",net_mac[0],net_mac[1],net_mac[2],net_mac[3],net_mac[4],net_mac[5]);

	asm_out_b(CR, (CR_PAGE1|CR_NODMA|CR_STOP));     // switch to page 1
	
	// write mac
	asm_out_b(PAR0, net_mac[0]);
	asm_out_b(PAR1, net_mac[1]);
	asm_out_b(PAR2, net_mac[2]);
	asm_out_b(PAR3, net_mac[3]);
	asm_out_b(PAR4, net_mac[4]);
	asm_out_b(PAR5, net_mac[5]);
		
	asm_out_b(CURR, RXSTART);                       // init curr pointer

	asm_out_b(CR, (CR_NODMA|CR_START));	            // start the NIC
}

void ne2000_send(uint8_t *packet, uint16_t length) {
	uint16_t i;
	
	if (length < 0x40) { length = 0x40; }
	i = 0;
	// If there is still a packet in transmission, wait until it's done!
	while ((i < 50000) && (asm_in_b(CR) & CR_TRANSMIT)) {
		i++;
	}
	/* Abort any currently running "DMA" operations */
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

uint16_t ne2000_recieve(uint8_t *packet, uint16_t max_size) {
	uint8_t curr, bnry;
	uint16_t i = 0;
	
	uint16_t net_recvdbytes;
	
	asm_out_b(CR, CR_PAGE1 | CR_START | CR_NODMA);	    // goto register page 1
	curr = asm_in_b(CURR);	                            // read the CURRent pointer
	
	asm_out_b(CR, CR_PAGE0 | CR_START | CR_NODMA);	    // goto register page 0
	
	// read the boundary register pointing to the beginning of the packet
	bnry = asm_in_b(BNRY);
	
	if( bnry == curr )
		return 0;
		
	// if boundary pointer is invalid
	if( (bnry >= RXSTOP) || (bnry < RXSTART) )
	{
		// reset the contents of the buffer and exit
		asm_out_b(BNRY, RXSTART);
		asm_out_b(CR, (CR_PAGE1|CR_NODMA|CR_START));
		asm_out_b(CURR, RXSTART);
		asm_out_b(CR, (CR_NODMA|CR_START));
		return 0;
	}
		
	if (bnry > RXSTOP - 1) { 
		bnry = RXSTART;
	}	

	asm_out_b(RBCR0, 0xff);
	asm_out_b(RBCR1, 0xff);
	asm_out_b(RSAR0, 0);                // low byte of start address (0)
	asm_out_b(RSAR1, bnry);             // high byte of start address (BNRY)
	
	// begin the dma read
	asm_out_b(CR, CR_PAGE0 | CR_START | CR_DMAREAD);
	
	asm_in_b(RDMA);
	
	bnry = asm_in_b(RDMA);              // next-pointer
	if (bnry < RXSTART) {
		bnry = RXSTOP;
	};
	
	net_recvdbytes = asm_in_b(RDMA);
	net_recvdbytes += (asm_in_b(RDMA)<<8);
	
	if (net_recvdbytes > MTU_SIZE) {
		net_recvdbytes = MTU_SIZE;
	}
	
	for (i = 0; i < net_recvdbytes; i++) {
		packet[i] = asm_in_b(RDMA);
	}
	
	asm_out_b(CR, CR_PAGE0 | CR_START | CR_NODMA);      // set page 0
	asm_out_b(BNRY, bnry);                              // write updated bnry pointer
	
	return net_recvdbytes;
}
