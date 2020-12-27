/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/console.h>
#include <dev/pci/pci.h>
#include <dev/pci/devicetree.h>
#include <panic/panic.h>

#define PCI_MAX_BUSSES 256
#define PCI_MAX_DEVICES_PER_BUS 32

void fill_pci_device(struct pci_device* dev, uint8_t bus, uint8_t device, uint8_t function){
	ASSERT_NOT_NULL(pci_devices, "pci_devices must not be null.  Did you init PCI?");
	ASSERT_NOT_NULL(dev, "dev must not be null");
	dev->bus = bus;
	dev->device = device;
	dev->function = function;
	dev->pci_class = pci_header_read_class(bus, device, function);
	dev->pci_subclass = pci_header_read_subclass(bus, device, function);
	dev->header_type = pci_header_read_type(bus, device, function);
	dev->irq = pci_header_read_irq(bus, device, function);
	dev->vendor_id = pci_header_read_vendor(bus, device, function);
	dev->device_id = pci_header_read_device_id(bus, device, function);
	dev->bars[0]= pci_header_read_bar0(bus, device, function);
	dev->bars[1]= pci_header_read_bar1(bus, device, function);
	dev->bars[2]= pci_header_read_bar2(bus, device, function);
	dev->bars[3]= pci_header_read_bar3(bus, device, function);
	dev->bars[4]= pci_header_read_bar4(bus, device, function);
	dev->bars[5]= pci_header_read_bar5(bus, device, function);
}

bool pci_device_exists(uint8_t bus, uint8_t device, uint8_t function){
	if (pci_header_read_vendor(bus, device, function) == 0xFFFF){
		return false;
	} else {
		return true;
	}
}

void pci_found_device(uint8_t bus, uint8_t device, uint8_t function){
	ASSERT_NOT_NULL(pci_devices, "pci_devices must not be null.  Did you init PCI?");
	uint8_t i;
	
	// new device
	struct pci_device* dev = kmalloc(sizeof(struct pci_device));
	
	// fill device
	fill_pci_device(dev , bus, device, function);

	// add to list
	list_add(pci_devices, dev);

	kprintf("PCI device found at %#hX:%#hX:%#hX, class %#hX.%#hX, vendor %#hX, device %#hX, IRQ %hu\n", dev->bus, dev->device, dev->function, dev->pci_class, dev->pci_subclass, dev->vendor_id, dev->device_id, dev->irq);
	
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
    ASSERT_NOT_NULL(pci_devices, "pci_devices must not be null.  Did you init PCI?");
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
	
	for (i = 0; i < PCI_MAX_BUSSES; i++){
		pci_scan_bus(i);
	}
}

void pci_scan_bus(uint8_t bus){
	uint8_t i;
	for (i = 0; i < PCI_MAX_DEVICES_PER_BUS; i++){
		if (pci_device_exists(bus, i, 0)){
			pci_found_device(bus, i, 0);
		}
	}
}

