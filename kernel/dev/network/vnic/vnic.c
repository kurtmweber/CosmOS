//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio*/

#include <dev/network/vnic/vnic.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <types.h>
#include <asm/io.h>
#include <sleep/sleep.h>
#include <dev/pci/pci.h>

#include <dev/network/vnic/virtio.h>

// virtio NIC flags
#define VIRTIO_NIC_MAC1             0x14
#define VIRTIO_NIC_MAC2             0x15
#define VIRTIO_NIC_MAC3             0x16
#define VIRTIO_NIC_MAC4             0x17
#define VIRTIO_NIC_MAC5             0x18
#define VIRTIO_NIC_MAC6             0x19
#define VIRTIO_NIC_STATUS           0x1A

// cut and paste these later into virtio block device
#define VIRTIO_BLOCK_TOTAL_SECTORS      0x14
#define VIRTIO_BLOCK_MAX_SEGMENT_SIZE   0x1C
#define VIRTIO_BLOCK_MAX_SEGMENT_COUNT  0x20
#define VIRTIO_BLOCK_CYLINDER_COUNT     0x24
#define VIRTIO_BLOCK_HEAD_COUNT         0x26
#define VIRTIO_BLOCK_SECTOR_COUNT       0x27
#define VIRTIO_BLOCK_LENGTH             0x28

void vnic_irq_handler(stackFrame *frame){
	kprintf("#");
}
/*
* perform device instance specific init here
*/
void VNICInit(struct device* dev){
    struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    interrupt_router_register_interrupt_handler(pci_dev->irq, &vnic_irq_handler);

    // TODO. There is stuff to merge and when that happens, bar0 will be in the pci dev struct
    uint64_t base = pci_header_read_bar0(pci_dev->bus, pci_dev->device,pci_dev->function);

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n",dev->description, pci_dev->irq,pci_dev->vendor_id, pci_dev->device_id, base, dev->name);
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
* find all NE2000 devices and register them
*/
void vnic_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x1AF4,0x1000, &VNICSearchCB);
}
