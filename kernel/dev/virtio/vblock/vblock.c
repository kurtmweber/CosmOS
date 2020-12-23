//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio*/

#include <dev/virtio/vblock/vblock.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <types.h>
#include <asm/io.h>
#include <sleep/sleep.h>
#include <dev/pci/pci.h>

#include <dev/virtio/virtio.h>

// cut and paste these later into virtio block device
#define VIRTIO_BLOCK_TOTAL_SECTORS      0x14
#define VIRTIO_BLOCK_MAX_SEGMENT_SIZE   0x1C
#define VIRTIO_BLOCK_MAX_SEGMENT_COUNT  0x20
#define VIRTIO_BLOCK_CYLINDER_COUNT     0x24
#define VIRTIO_BLOCK_HEAD_COUNT         0x26
#define VIRTIO_BLOCK_SECTOR_COUNT       0x27
#define VIRTIO_BLOCK_LENGTH             0x28

// TODO move this into virtio block device
struct BlockRequest {
  uint32_t Type;              // 0: Read; 1: Write; 4: Flush; 11: Discard; 13: Write zeroes
  uint32_t Reserved;
  uint64_t Sector;
  uint8_t Data[0];             // Data's size must be a multiple of 512
  uint8_t Status;             // 0: OK; 1: Error; 2: Unsupported
};

void vblock_irq_handler(stackFrame *frame){
	kprintf("#");
}
/*
* perform device instance specific init here
*/
void VBLOCKInit(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    interrupt_router_register_interrupt_handler(pci_dev->irq, &vblock_irq_handler);

    // TODO. There is stuff to merge and when that happens, bar0 will be in the pci dev struct
    uint64_t base = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id, base, dev->name);
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
