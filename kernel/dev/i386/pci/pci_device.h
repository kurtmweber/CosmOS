//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PCI_DEVICE_H
#define _PCI_DEVICE_H

#include <dev/i386/pci/devicetree.h>
#include <sys/collection/arraylist/arraylist.h>
#include <types.h>

struct pci_device {
    pci_class_codes pci_class;
    uint8_t pci_subclass;
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint8_t header_type;
    uint8_t irq;
    uint16_t vendor_id;
    uint16_t device_id;
    uint32_t bars[6];
};

// search for PCI devices
typedef void (*pcideviceSearchCallback)(struct pci_device* dev);
void pci_devicemgr_search_device(pci_class_codes pci_class, uint8_t pci_subclass, uint16_t vendor_id, uint16_t device_id, pcideviceSearchCallback cb);
void pci_devicemgr_search_devicetype(pci_class_codes pci_class, uint8_t pci_subclass, pcideviceSearchCallback cb);

// calc PCI base address
uint64_t pci_calcbar(struct pci_device* pci_dev);

extern struct arraylist* pci_devices;

#endif
