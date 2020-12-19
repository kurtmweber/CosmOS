/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PCI_H
#define _PCI_H

#include <pci/devicetree.h>
#include <pci/pci_device.h>

#define PCI_CONFIG_ADDRESS_PORT	0xCF8
#define PCI_CONFIG_DATA_PORT	0xCFC

#define PCI_BAR0_OFFSET		0x10
#define PCI_BAR1_OFFSET		0x14
#define PCI_BAR2_OFFSET		0x18
#define PCI_BAR3_OFFSET		0x1C
#define PCI_BAR4_OFFSET		0x20
#define PCI_BAR5_OFFSET		0x24

uint32_t pci_config_address_build(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t enabled);

uint32_t pci_header_read_bar0(uint8_t bus, uint8_t device, uint8_t function);
uint32_t pci_header_read_bar1(uint8_t bus, uint8_t device, uint8_t function);
uint32_t pci_header_read_bar2(uint8_t bus, uint8_t device, uint8_t function);
uint32_t pci_header_read_bar3(uint8_t bus, uint8_t device, uint8_t function);
uint32_t pci_header_read_bar4(uint8_t bus, uint8_t device, uint8_t function);
uint32_t pci_header_read_bar5(uint8_t bus, uint8_t device, uint8_t function);
pci_class_codes pci_header_read_class(uint8_t bus, uint8_t device, uint8_t function);
uint16_t pci_header_read_device_id(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_header_read_irq(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_header_read_subclass(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pci_header_read_type(uint8_t bus, uint8_t device, uint8_t function);
uint16_t pci_header_read_vendor(uint8_t bus, uint8_t device, uint8_t function);

void pci_header_set_irq(uint8_t bus, uint8_t device, uint8_t function, uint8_t irq);

void pci_init();

void pci_scan();
void fill_pci_device(struct pci_device* dev, uint8_t bus, uint8_t device, uint8_t function);
void pci_found_device(uint8_t bus, uint8_t device, uint8_t function);
void pci_scan_bus(uint8_t bus);
bool pci_device_exists(uint8_t bus, uint8_t device, uint8_t function);

#endif