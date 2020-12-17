//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PCI_DEVICE_H
#define _PCI_DEVICE_H

#include <types.h>
#include <pci/devicetree.h>

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
};

// search for PCI devices
typedef void (*deviceSearchCallback)(struct pci_device* dev);
void pci_search_device(pci_class_codes pci_class, uint8_t pci_subclass, uint16_t vendor_id, uint16_t device_id, deviceSearchCallback cb);
void pci_search_devicetype(pci_class_codes pci_class, uint8_t pci_subclass, deviceSearchCallback cb);

extern struct pci_device *pci_devices;
extern uint16_t num_pci_devices;

#endif

