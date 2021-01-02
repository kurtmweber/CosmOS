//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio

#include <dev/i386/virtio/vnic/vnic.h>
#include <system/platform/i386/interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <system/devicemgr/devicemgr.h>
#include <system/console/console.h>
#include <types.h>
#include <asm/io.h>
#include <system/sleep/sleep.h>
#include <dev/i386/pci/pci.h>
#include <debug/assert.h>
#include <dev/i386/virtio/virtio.h>
#include <system/devicemgr/deviceapi/deviceapi_ethernet.h>
#include <dev/i386/virtio/virtqueue.h>

#define VNIC_QUEUE_SIZE 255

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
/*
* vnic instance specific data
*/
struct vnic_devicedata {
    uint64_t base;
    struct virtq* send_queue;
    struct virtq* recieve_queue;
} __attribute__((packed));

void vnic_irq_handler(stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
	kprintf("#");
}

//void vinc_write_queue_notify_register(uint16_t status){
//    asm_out_w(deviceData->base+VIRTIO_QUEUE_NOTIFY,status);
//}

//uint8_t vnic_get_status(){
//  return asm_in_b(deviceData->base+VIRTIO_NIC_STATUS);
//}

/*
* perform device instance specific init here
*/
void VNICInit(struct device* dev){
	  ASSERT_NOT_NULL(dev, "dev cannot be null");
	  ASSERT_NOT_NULL(dev->deviceData, "dev->deviceData cannot be null");

    struct vnic_devicedata* deviceData = (struct vnic_devicedata*) dev->deviceData;
    interrupt_router_register_interrupt_handler(dev->pci->irq, &vnic_irq_handler);    
    deviceData->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);

    // make the recieve queue
    struct virtq* recieve_q = virtq_new(VNIC_QUEUE_SIZE);
    bool recieve_q_aligned = virtio_isAligned(((uint64_t)recieve_q),4096);
    ASSERT(recieve_q_aligned, "recieve_q is not 4096 byte aligned");
    deviceData->recieve_queue = recieve_q;
    
    // make the send queue
    struct virtq* send_q = virtq_new(VNIC_QUEUE_SIZE);
    bool send_q_aligned = virtio_isAligned(((uint64_t)send_q),4096);
    ASSERT(send_q_aligned, "send_q is not 4096 byte aligned");
    deviceData->send_queue = send_q;

    uint8_t virtio_mac[6];
    virtio_mac[0] = asm_in_b(deviceData->base+VIRTIO_NIC_MAC1);
    virtio_mac[1] = asm_in_b(deviceData->base+VIRTIO_NIC_MAC2);
    virtio_mac[2] = asm_in_b(deviceData->base+VIRTIO_NIC_MAC3);
    virtio_mac[3] = asm_in_b(deviceData->base+VIRTIO_NIC_MAC4);
    virtio_mac[4] = asm_in_b(deviceData->base+VIRTIO_NIC_MAC5);
    virtio_mac[5] = asm_in_b(deviceData->base+VIRTIO_NIC_MAC6);

    /*
    * should start with 0x52, 0x54
    */
    if ((virtio_mac[0] !=0x52) ||(virtio_mac[1] !=0x54)){
      panic("Unexpected virtio MAC address");
    }

	  kprintf("   MAC %#hX:%#hX:%#hX:%#hX:%#hX:%#hX\n",virtio_mac[0],virtio_mac[1],virtio_mac[2],virtio_mac[3],virtio_mac[4],virtio_mac[5]);
}

void vnic_ethernet_read(struct device* dev, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");
	panic("vnic read not implemented yet");
}

void vnic_ethernet_write(struct device* dev, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");

	panic("vnic write not implemented yet");
}

void vnic_search_cb(struct pci_device* dev){
    ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &VNICInit;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = VNIC;
    devicemgr_set_device_description(deviceinstance, "Virtio NIC");
    /*
    * the device api
    */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*) kmalloc(sizeof(struct deviceapi_ethernet));
    api->write = &vnic_ethernet_read;
    api->read = &vnic_ethernet_write;
    deviceinstance->api = api;
  	/*
    * device data
    */
    struct vnic_devicedata* deviceData = (struct vnic_devicedata*) kmalloc(sizeof(struct vnic_devicedata));
    deviceinstance->deviceData = deviceData;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}

/**
* find all virtio ethernet devices and register them
*/
void vnic_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,VIRTIO_PCI_MANUFACTURER,VIRTIO_PCI_DEVICED_NETWORK, &vnic_search_cb);
}
