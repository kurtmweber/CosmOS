//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NE2000PCI_H
#define _NE2000PCI_H

#include <types.h>

void ne2000pci_devicemgr_register_devices();

void ne2000pci_send(uint8_t *packet, uint16_t length);
uint16_t ne2000pci_recieve(uint8_t *packet, uint16_t max_size);

#endif