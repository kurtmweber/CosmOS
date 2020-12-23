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

#define VIRTIO_BLOCK_TOTAL_SECTORS      0x14
#define VIRTIO_BLOCK_MAX_SEGMENT_SIZE   0x1C
#define VIRTIO_BLOCK_MAX_SEGMENT_COUNT  0x20
#define VIRTIO_BLOCK_CYLINDER_COUNT     0x24
#define VIRTIO_BLOCK_HEAD_COUNT         0x26
#define VIRTIO_BLOCK_SECTOR_COUNT       0x27
#define VIRTIO_BLOCK_LENGTH             0x28

uint64_t vblock_base=0;

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
	kprintf("#");
}

// https://wiki.osdev.org/PCI
uint64_t calcbar( struct pci_device* pci_dev){
   uint64_t bar0 = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);
   uint64_t bar1 = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);

   return  ((bar0 & 0xFFFFFFF0) + ((bar1 & 0xFFFFFFFF) << 32)) ;
}

/*
* perform device instance specific init here
*/
void VBLOCKInit(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    interrupt_router_register_interrupt_handler(pci_dev->irq, &vblock_irq_handler);

    // TODO. There is stuff to merge and when that happens, bar0 will be in the pci dev struct
   // vblock_base = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);
    vblock_base = calcbar(pci_dev);

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id, vblock_base, dev->name);

    uint32_t totalSectors = asm_in_d(vblock_base+VIRTIO_BLOCK_TOTAL_SECTORS);
    kprintf("Total sectors %llu\n", totalSectors);

    // acknowledge device and set the driver loaded bit
    asm_out_b(vblock_base+VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED);

    // get the feature bits
    uint32_t features = asm_in_b(vblock_base+VIRTIO_DEVICE_FEATURES);
    kprintf("Features %llu\n", features);

    // for now, we dont use any features
  //  asm_out_b(vblock_base+VIRTIO_DEVICE_FEATURES,0x00);

    // write features ok
  //  asm_out_b(vblock_base+VIRTIO_DEVICE_STATUS,VIRTIO_STATUS_FEATURES_OK);

    // read fearures ok.  we good?
  //  uint32_t status = asm_in_b(vblock_base+VIRTIO_DEVICE_STATUS);
  //  kprintf("Features %llu\n", status);

}


void VBLOCKSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &VBLOCKInit;
    deviceinstance->deviceData = dev;
    deviceinstance->devicetype = ATA;
    devicemgr_set_device_description(deviceinstance, "Virtio ATA");
    devicemgr_register_device(deviceinstance);
}

/**
* find all virtio block devices and register them
*/
void vblock_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_MASS_STORAGE,PCI_MASS_STORAGE_SUBCLASS_SCSI,0x1AF4,0x1001, &VBLOCKSearchCB);
}
