//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio

#include <dev/virtio/vblock/vblock.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <types.h>
#include <asm/io.h>
#include <sleep/sleep.h>
#include <dev/pci/pci.h>
#include <mm/mm.h>
#include <dev/virtio/virtio.h>
#include <panic/assert.h>
#include <dev/virtio/virtqueue.h>

// registers
#define VIRTIO_BLOCK_TOTAL_SECTORS      0x14
#define VIRTIO_BLOCK_MAX_SEGMENT_SIZE   0x1C
#define VIRTIO_BLOCK_MAX_SEGMENT_COUNT  0x20
#define VIRTIO_BLOCK_CYLINDER_COUNT     0x24
#define VIRTIO_BLOCK_HEAD_COUNT         0x26
#define VIRTIO_BLOCK_SECTOR_COUNT       0x27
#define VIRTIO_BLOCK_LENGTH             0x28

// feature flags
#define VIRTIO_BLK_F_SIZE_MAX       1
#define VIRTIO_BLK_F_SEG_MAX        2
#define VIRTIO_BLK_F_GEOMETRY       4
#define VIRTIO_BLK_F_RO             5
#define VIRTIO_BLK_F_BLK_SIZE       6
#define VIRTIO_BLK_F_FLUSH          9
#define VIRTIO_BLK_F_TOPOLOGY       10
#define VIRTIO_BLK_F_CONFIG_WCE     11
#define VIRTIO_BLK_F_DISCARD        13
#define VIRTIO_BLK_F_WRITE_ZEROES   14

// request types
#define VIRTIO_BLK_T_IN 0
#define VIRTIO_BLK_T_OUT 1
#define VIRTIO_BLK_T_FLUSH 4
#define VIRTIO_BLK_T_DISCARD 11
#define VIRTIO_BLK_T_WRITE_ZEROES 13

/*
* vblock instance specific data
*/
struct vblock_devicedata {
    uint64_t base;
    uint32_t sectorLength;
    struct virtq* vblock_queue;
} __attribute__((packed));

struct vblock_block_request {
  uint32_t type;              // 0: Read; 1: Write; 4: Flush; 11: Discard; 13: Write zeroes
  uint32_t reserved;
  uint64_t sector;
  uint8_t* data;             // Data's size must be a multiple of 512
  uint8_t  status;             // 0: OK; 1: Error; 2: Unsupported
};

struct vblock_block_request* vblock_block_request_new() {
    struct vblock_block_request* ret = kmalloc(sizeof(struct vblock_block_request));   
    ret->data=0;
    ret->type=0;
    ret->reserved=0;
    ret->sector=0;
    ret->status=0;
    return ret;
}

void vblock_irq_handler(stackFrame *frame){
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
	kprintf("#");
}

/*
* perform device instance specific init here
*/
void VBLOCKInit(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(dev->deviceData, "dev->deviceData cannot be null");

    struct vblock_devicedata* deviceData = (struct vblock_devicedata*) dev->deviceData;
    interrupt_router_register_interrupt_handler(dev->pci->irq, &vblock_irq_handler);
    deviceData->base = pci_calcbar(dev->pci);

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);

    // acknowledge device and set the driver loaded bit
    asm_out_b(deviceData->base+VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED);

    // get the feature bits
    uint32_t features = asm_in_b(deviceData->base+VIRTIO_DEVICE_FEATURES);

    // we kinda care about geometry and block size
    asm_out_b(deviceData->base+VIRTIO_GUEST_FEATURES,VIRTIO_BLK_F_GEOMETRY|VIRTIO_BLK_F_BLK_SIZE);

    // write features ok
    asm_out_b(deviceData->base+VIRTIO_DEVICE_STATUS,VIRTIO_STATUS_FEATURES_OK);

    // read features ok.  we good?
    uint32_t status = asm_in_b(deviceData->base+VIRTIO_DEVICE_STATUS);
    if (VIRTIO_STATUS_FEATURES_OK!=status) {
      panic("virtio feature negotiation failed");
    }

    // cool
    asm_out_b(deviceData->base+VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DRIVER_READY);

    /*
    * length*totalSectors should equal the byte size of the mounted file (currently hda.img)
    */
    uint32_t totalSectors = asm_in_d(deviceData->base+VIRTIO_BLOCK_TOTAL_SECTORS);
    deviceData->sectorLength = asm_in_d(deviceData->base+VIRTIO_BLOCK_LENGTH);
    uint64_t totalBytes = totalSectors*(deviceData->sectorLength);
    kprintf("   Total byte size of mounted media: %llu\n",totalBytes);

    // make the queue
    deviceData->vblock_queue = virtq_new();

    // set the queue.  The API takes a 32 bit pointer, but we have a 64 bit pointer, so ... some conversions  
    kprintf("   Queue Address: %#hX\n", (uint64_t) deviceData->vblock_queue);
    asm_out_d(VIRTIO_QUEUE_ADDRESS, (uint32_t) (uint64_t) deviceData->vblock_queue);
    asm_out_w(VIRTIO_QUEUE_SIZE, VIRTQUEUE_SIZE);
}

void VBLOCKSearchCB(struct pci_device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &VBLOCKInit;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = ATA;
    devicemgr_set_device_description(deviceinstance, "Virtio ATA");
	/*
	* device data
	*/
	struct vblock_devicedata* deviceData = (struct vblock_devicedata*) kmalloc(sizeof(struct vblock_devicedata));
	deviceinstance->deviceData = deviceData;
    devicemgr_register_device(deviceinstance);
}

/**
* find all virtio block devices and register them
*/
void vblock_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_MASS_STORAGE,PCI_MASS_STORAGE_SUBCLASS_SCSI,VIRTIO_PCI_MANUFACTURER,VIRTIO_PCI_DEVICED_BLOCK, &VBLOCKSearchCB);
}

void vblock_read(struct device* dev, uint32_t sector, uint8_t* target, uint32_t size) {
    ASSERT_NOT_NULL(dev->deviceData, "dev->deviceData cannot be null");
    struct vblock_devicedata* deviceData = (struct vblock_devicedata*) dev->deviceData;

    /*
    * block request
    */
    struct vblock_block_request* req = vblock_block_request_new();
    req->type = VIRTIO_BLK_T_IN;
    req->sector = sector;
    req->data = kmalloc(deviceData->sectorLength);

    /*
    * descriptor
    */
    struct virtq_descriptor* desc = virtq_descriptor_new((uint8_t*)req, sizeof(struct vblock_block_request));
    // 0 for read, 1 for write
    desc->flags=0;

    // enqueue
    virtq_enqueue_descriptor(deviceData->vblock_queue, desc);
}

