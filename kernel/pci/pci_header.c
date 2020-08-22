/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PCI_HEADER_C
#define _PCI_HEADER_C

#include <types.h>
#include <asm/asm.h>
#include <pci/pci.h>

uint16_t pci_header_read_vendor(uint8_t bus, uint8_t device){
	uint32_t register_dword;
	
	asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, 0, 0, 1));
	register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
	
	return (uint16_t)(register_dword & 0x0000FFFF);
}

#endif