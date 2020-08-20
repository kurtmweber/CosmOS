/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PCI_H
#define _PCI_H

#define PCI_CONFIG_ADDRESS_PORT	0xCF8
#define PCI_CONFIG_DATA_PORT	0xCFC

#ifndef _PCI_ADDRESS_C
uint32_t pci_config_address_build(uint8_t offset, uint8_t function, uint8_t device, uint8_t bus, uint8_t enabled);
#endif

#ifndef _PCI_INIT_C
void pci_init();
#endif

#ifndef _PCI_SCAN_C
void pci_scan();
#endif


#endif