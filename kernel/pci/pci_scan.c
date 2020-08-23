/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PCI_SCAN_C
#define _PCI_SCAN_C

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <pci/pci.h>
#include <pci/devicetree.h>

pci_device_t fill_pci_device(uint8_t bus, uint8_t device, uint8_t function){
	// ensure the given device actually exists BEFORE sending it to this function
	
	pci_device_t tmp;
	
	tmp.bus = bus;
	tmp.device = device;
	tmp.function = function;
	tmp.pci_class = pci_header_read_class(bus, device, function);
	tmp.pci_subclass = pci_header_read_subclass(bus, device, function);
	tmp.header_type = pci_header_read_type(bus, device, function);
	
	return tmp;
}

bool pci_device_exists(uint8_t bus, uint8_t device, uint8_t function){
	if (pci_header_read_vendor(bus, device, function) == 0xFFFF){
		return false;
	} else {
		return true;
	}
}

void pci_found_device(uint8_t bus, uint8_t device, uint8_t function){
	uint8_t i;
	
	num_pci_devices++;
	
	if (!pci_devices){
		pci_devices = kmalloc(sizeof(pci_device_t) * num_pci_devices);
	} else {
		pci_devices = krealloc(pci_devices, sizeof(pci_device_t) * num_pci_devices);
	}
	
	pci_devices[num_pci_devices - 1] = fill_pci_device(bus, device, function);
	kprintf("PCI device found at %#hX:%#hX:%#hX, class %#hX.%#hX\n", pci_devices[num_pci_devices - 1].bus, pci_devices[num_pci_devices - 1].device, pci_devices[num_pci_devices - 1].function, pci_devices[num_pci_devices - 1].pci_class, pci_devices[num_pci_devices - 1].pci_subclass);
	
	if ((pci_header_read_type(bus, device, function) & 0x80) == 0x80){
		for (i = 1; i < 8; i++){
			if (pci_device_exists(bus, device, i)){
				pci_found_device(bus, device, i);
			}
		}
	}
	
	return;
}

void pci_scan(){
	uint16_t bus = 0, device = 0, function = 0;
	uint32_t register_dword;
	uint16_t device_id, vendor_id;
	uint8_t class_code, subclass_code, header_code;
	uint16_t i;
	
	kprintf("Scanning for PCI devices...\n");
	
	/*for (bus = 0; bus < 256; bus++){
		for (device = 0; device < 32; device++){
			for (function = 0; function < 8; function++){
				asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0, 1));
				register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
				
				vendor_id = (uint16_t)(register_dword & 0x0000FFFF);
				device_id = (uint16_t)(register_dword >> 16);
				
				if (vendor_id != 0xFFFF){
					kprintf("PCI device found at %#hX:%#hX:%#hX, vendor %#X, device %#X\n", bus, device, function, vendor_id, device_id);
					asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0x08, 1));
					register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
					
					class_code = (uint8_t)(register_dword >> 24);
					subclass_code = (uint8_t)((register_dword >> 16) & 0x00FF);
					
					asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0x0C, 1));
					register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
					header_code = (uint8_t)((register_dword >> 16) & 0x00FF);
					
					kprintf("\tClass %#hX, subclass %#hX, header type %#hX\n", class_code, subclass_code, header_code);
				}
			}
		}
	}*/
	
	for (i = 0; i < 256; i++){
		pci_scan_bus(i);
	}
	
	return;
}

void pci_scan_bus(uint8_t bus){
	uint8_t i;
	
	for (i = 0; i < 32; i++){
		if (pci_device_exists(bus, i, 0)){
			pci_found_device(bus, i, 0);
		}
		
	}
	
	return;
}

#endif