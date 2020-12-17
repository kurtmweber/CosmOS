
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PCI_DEVICE_H
#define _PCI_DEVICE_H

#include <pci/pci_device.h>
#include <types.h>

struct pci_device *pci_devices;
uint16_t num_pci_devices;

#endif