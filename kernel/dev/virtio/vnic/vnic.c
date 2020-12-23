//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio

#include <dev/virtio/vnic/vnic.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <types.h>
#include <asm/io.h>
#include <sleep/sleep.h>
#include <dev/pci/pci.h>
#include <panic/panic.h>
#include <dev/virtio/virtio.h>

// virtio NIC flags
#define VIRTIO_NIC_MAC1             0x14
#define VIRTIO_NIC_MAC2             0x15
#define VIRTIO_NIC_MAC3             0x16
#define VIRTIO_NIC_MAC4             0x17
#define VIRTIO_NIC_MAC5             0x18
#define VIRTIO_NIC_MAC6             0x19
#define VIRTIO_NIC_STATUS           0x1A

struct VirtioPacketHeader {
  uint8_t Flags;                // Bit 0: Needs checksum; Bit 1: Received packet has valid data;
                                // Bit 2: If VIRTIO_NET_F_RSC_EXT was negotiated, the device processes
                                // duplicated ACK segments, reports number of coalesced TCP segments in ChecksumStart
                                // field and number of duplicated ACK segments in ChecksumOffset field,
                                // and sets bit 2 in Flags(VIRTIO_NET_HDR_F_RSC_INFO) 
  uint8_t SegmentationOffload;  // 0:None 1:TCPv4 3:UDP 4:TCPv6 0x80:ECN
  uint16_t HeaderLength;        // Size of header to be used during segmentation.
  uint16_t SegmentLength;       // Maximum segment size (not including header).
  uint16_t ChecksumStart;       // The position to begin calculating the checksum.
  uint16_t ChecksumOffset;      // The position after ChecksumStart to store the checksum.
  uint16_t BufferCount;         // Used when merging buffers.
};

void vnic_irq_handler(stackFrame *frame){
	kprintf("#");
}

uint64_t vnic_base=0;

void vinc_write_queue_notify_register(uint16_t status){
    asm_out_w(vnic_base+VIRTIO_QUEUE_NOTIFY,status);
}

uint8_t vnic_get_status(){
  return asm_in_b(vnic_base+VIRTIO_NIC_STATUS);
}

uint64_t vnic_calcbar( struct pci_device* pci_dev){
   uint64_t bar0 = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);
   uint64_t bar1 = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);

   return  ((bar0 & 0xFFFFFFF0) + ((bar1 & 0xFFFFFFFF) << 32)) ;
}

/*
* perform device instance specific init here
*/
void VNICInit(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    interrupt_router_register_interrupt_handler(pci_dev->irq, &vnic_irq_handler);

    // TODO. There is stuff to merge and when that happens, bar0 will be in the pci dev struct

    
   // vnic_base = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);
 vnic_base = vnic_calcbar(pci_dev);


    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id, vnic_base, dev->name);

    uint8_t virtio_mac[6];
    virtio_mac[0] = asm_in_b(vnic_base+VIRTIO_NIC_MAC1);
    virtio_mac[1] = asm_in_b(vnic_base+VIRTIO_NIC_MAC2);
    virtio_mac[2] = asm_in_b(vnic_base+VIRTIO_NIC_MAC3);
    virtio_mac[3] = asm_in_b(vnic_base+VIRTIO_NIC_MAC4);
    virtio_mac[4] = asm_in_b(vnic_base+VIRTIO_NIC_MAC5);
    virtio_mac[5] = asm_in_b(vnic_base+VIRTIO_NIC_MAC6);

    /*
    * should start with 0x52, 0x54
    */
    if ((virtio_mac[0] !=0x52) ||(virtio_mac[1] !=0x54)){
      panic("Unexpected virtio MAC address");
    }

	  kprintf("MAC %#hX:%#hX:%#hX:%#hX:%#hX:%#hX\n",virtio_mac[0],virtio_mac[1],virtio_mac[2],virtio_mac[3],virtio_mac[4],virtio_mac[5]);
}

void VNICSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &VNICInit;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "Virtio NIC");
    devicemgr_register_device(deviceinstance);
}

/**
* find all virtio ethernet devices and register them
*/
void vnic_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x1AF4,0x1000, &VNICSearchCB);
}
