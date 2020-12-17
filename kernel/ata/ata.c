/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <ata/ata.h>
#include <console/console.h>
#include <mm/mm.h>
#include <pci/pci.h>
#include <device/device.h>

ide_controller_t *ide_controllers;

void ata_detect_addresses(uint16_t num_ide){
	uint16_t i;
	uint32_t bar_result;
	
	for (i = 0; i < num_ide; i++){
		bar_result = pci_header_read_bar0(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_PRIMARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x1F0 : bar_result);
		
		bar_result = pci_header_read_bar1(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_PRIMARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x3F6 : bar_result);
		
		bar_result = pci_header_read_bar2(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_SECONDARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x170 : bar_result);
		
		bar_result = pci_header_read_bar3(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_SECONDARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x376 : bar_result);
	}
}

void ata_init(){
	uint16_t i = 0;
	uint16_t num_ide;
	
	kprintf("Initializing ATA devices...\n");
	
	ide_controllers = (ide_controller_t *)0;
	
	num_ide = ata_scan_ide_controllers();
	if (!num_ide){
		kprintf("No IDE controllers detected\n");
		return;
	}
	
	for (i = 0; i < num_ide; i++){
		ide_controllers[i].channels[IDE_CHANNEL_PRIMARY].selected_device = ATA_DRIVE_SELECT_NONE;
		ide_controllers[i].channels[IDE_CHANNEL_SECONDARY].selected_device = ATA_DRIVE_SELECT_NONE;
	}
	
	ata_detect_addresses(num_ide);
	for (i = 0; i < num_ide; i++){
		kprintf("Primary IDE I/O at %#X, control at %#X\n", ide_controllers[i].channels[IDE_CHANNEL_PRIMARY].base_io, ide_controllers[i].channels[IDE_CHANNEL_PRIMARY].base_io_ctrl);
		kprintf("Secondary IDE I/O at %#X, control at %#X\n", ide_controllers[i].channels[IDE_CHANNEL_SECONDARY].base_io, ide_controllers[i].channels[IDE_CHANNEL_SECONDARY].base_io_ctrl);
	}
	
	ata_setup_irq(num_ide);
	
	for (i = 0; i < num_ide; i++){
		ata_interrupt_enable(i, IDE_CHANNEL_PRIMARY, false);
		ata_interrupt_enable(i, IDE_CHANNEL_SECONDARY, false);
	}
	
	for (i = 0; i < num_ide; i++){
		ata_detect_devices(i);
	}
	
	return;
}

void deviceInitATA(){
    kprintf("Init ATA\n");
}

uint16_t ata_scan_ide_controllers(){
	uint16_t i = 0, num_ide = 0;
	bool ide_found = false;
	
	for (i = 0; i < num_pci_devices; i++){
		if ((pci_devices[i].pci_class == PCI_CLASS_MASS_STORAGE) && ((pci_mass_storage_subclass_codes)pci_devices[i].pci_subclass == PCI_MASS_STORAGE_SUBCLASS_IDE)){
			kprintf("IDE Controller found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
			kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
			
			num_ide++;
			
			if (!ide_controllers){
				ide_controllers = kmalloc(sizeof(ide_controller_t));
			} else {
				ide_controllers = krealloc(ide_controllers, num_ide * sizeof(ide_controller_t));
			}
			
			ide_controllers[num_ide - 1].pci = &pci_devices[i];

			/*
			* register device
			*/
			struct device* deviceinstance = newDevice();
			deviceinstance->init =  &deviceInitATA;
			registerDevice(deviceinstance);
		}
	}
	
	return num_ide;
}

void ata_setup_irq(uint16_t num_ide){
	uint8_t in;
	uint16_t i;
	
	// if this doesn't set the IRQ then this is a parallel IDE, but we don't need to know that
	
	for (i = 0; i < num_ide; i++){
		pci_header_set_irq(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function, IDE_SERIAL_IRQ);
	}
}
