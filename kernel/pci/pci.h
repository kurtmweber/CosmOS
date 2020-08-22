/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PCI_H
#define _PCI_H

#include <pci/devicetree.h>

#define PCI_CONFIG_ADDRESS_PORT	0xCF8
#define PCI_CONFIG_DATA_PORT	0xCFC

#ifndef _PCI_ADDRESS_C
uint32_t pci_config_address_build(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t enabled);
#endif

#ifndef _PCI_HEADER_C
uint16_t pci_header_read_vendor(uint8_t bus, uint8_t device);
#endif

#ifndef _PCI_INIT_C
void pci_init();
#endif

#ifndef _PCI_SCAN_C
void pci_scan();
#else
void pci_scan_bus(uint8_t bus);
bool pci_device_exists(uint8_t bus, uint8_t device);
#endif


#endif