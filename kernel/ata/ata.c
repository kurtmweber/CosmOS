/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_C
#define _ATA_C

#include <types.h>
#include <ata/ata.h>
#include <console/console.h>
#include <mm/mm.h>
#include <pci/pci.h>

void ata_init(){
	uint16_t i = 0, num_ide = 0;
	kprintf("Initializing ATA devices...\n");
	bool ide_found = false;
	uint32_t bar_result;
	
	ide_controllers = (ide_controller_t *)0;
	
	for (i = 0; i < num_pci_devices; i++){
		if ((pci_devices[i].pci_class == PCI_CLASS_MASS_STORAGE) && ((pci_mass_storage_subclass_codes)pci_devices[i].pci_subclass == PCI_MASS_STORAGE_SUBCLASS_IDE)){
			kprintf("IDE Controller found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
			
			num_ide++;
			
			if (!ide_controllers){
				ide_controllers = kmalloc(sizeof(ide_controller_t));
			} else {
				ide_controllers = krealloc(ide_controllers, num_ide * sizeof(ide_controller_t));
			}
			
			ide_controllers[num_ide - 1].pci = &pci_devices[i];
			
			ide_found = true;
		}
	}
	
	if (!ide_found){
		kprintf("No IDE controllers detected\n");
		return;
	}
	
	for (i = 0; i < num_ide; i++){
		bar_result = pci_header_read_bar0(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_PRIMARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x1F0 : bar_result);
		
		bar_result = pci_header_read_bar1(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_PRIMARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x3F6 : bar_result);
		
		bar_result = pci_header_read_bar2(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_SECONDARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x170 : bar_result);
		
		bar_result = pci_header_read_bar3(ide_controllers[i].pci->bus, ide_controllers[i].pci->device, ide_controllers[i].pci->function);
		ide_controllers[i].channels[IDE_CHANNEL_SECONDARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x376 : bar_result);
	}
	
	return;
}

#endif