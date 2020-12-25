/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <dev/ata/ata.h>
#include <console/console.h>
#include <mm/mm.h>
#include <dev/pci/pci.h>
#include <devicemgr/devicemgr.h>
#include <devicemgr/deviceapi/deviceapi_ata.h>
#include <panic/panic.h>

struct list *ide_controllers;

void ata_setup_irq();

void ata_detect_addresses(){
	uint16_t i;
	uint32_t bar_result;
	
	for (i = 0; i < NUM_CONTROLLERS; i++) {
		uint8_t bus =ATA_CONTROLLER(i)->pci->bus;
		uint8_t device = ATA_CONTROLLER(i)->pci->device;
		uint8_t function = ATA_CONTROLLER(i)->pci->function;

		bar_result = pci_header_read_bar0(bus, device, function);
		ATA_CONTROLLER(i)->channels[IDE_CHANNEL_PRIMARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x1F0 : bar_result);
		
		bar_result = pci_header_read_bar1(bus, device, function);
		ATA_CONTROLLER(i)->channels[IDE_CHANNEL_PRIMARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x3F6 : bar_result);
		
		bar_result = pci_header_read_bar2(bus, device, function);
		ATA_CONTROLLER(i)->channels[IDE_CHANNEL_SECONDARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x170 : bar_result);
		
		bar_result = pci_header_read_bar3(bus, device, function);
		ATA_CONTROLLER(i)->channels[IDE_CHANNEL_SECONDARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x376 : bar_result);
	}
}

void deviceInitATA(struct device* dev){
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, dev->name);
	if (0==NUM_CONTROLLERS){
		kprintf("No IDE controllers detected\n");
		return;
	}
	uint16_t i;
	for (i = 0; i < NUM_CONTROLLERS; i++){
		ATA_CONTROLLER(i)->channels[IDE_CHANNEL_PRIMARY].selected_device = ATA_DRIVE_SELECT_NONE;
		ATA_CONTROLLER(i)->channels[IDE_CHANNEL_SECONDARY].selected_device = ATA_DRIVE_SELECT_NONE;
	}

	ata_detect_addresses();

	for (i = 0; i < NUM_CONTROLLERS; i++){
		kprintf("Primary IDE I/O at %#X, control at %#X\n", ATA_CONTROLLER(i)->channels[IDE_CHANNEL_PRIMARY].base_io, ATA_CONTROLLER(i)->channels[IDE_CHANNEL_PRIMARY].base_io_ctrl);
		kprintf("Secondary IDE I/O at %#X, control at %#X\n", ATA_CONTROLLER(i)->channels[IDE_CHANNEL_SECONDARY].base_io, ATA_CONTROLLER(i)->channels[IDE_CHANNEL_SECONDARY].base_io_ctrl);
	}
	
	ata_setup_irq();
	
	for (i = 0; i < NUM_CONTROLLERS; i++){
		ata_interrupt_enable(i, IDE_CHANNEL_PRIMARY, false);
		ata_interrupt_enable(i, IDE_CHANNEL_SECONDARY, false);
	}
	
	for (i = 0; i < NUM_CONTROLLERS; i++){
		ata_detect_devices(i);
	}
	
	return;
}

void ata_read(struct device* dev, uint32_t sector, uint8_t* data, uint8_t* size) {
	panic("ATA read not implemented yet");
}
void ata_write(struct device* dev, uint32_t sector, uint8_t* data, uint8_t* size) {
	panic("ATA write not implemented yet");
}

void ATASearchCB(struct pci_device* dev){
	/*
	* save in IDE list
	*/
	struct ide_controller_t* ide_controller = kmalloc(sizeof(struct ide_controller_t));
	ide_controller->pci = dev;
	list_add(ide_controllers, ide_controller);

    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &deviceInitATA;
    deviceinstance->pci = dev;
	deviceinstance->devicetype=ATA;
	devicemgr_set_device_description(deviceinstance, "ATA");
	/*
    * the device api
    */
    struct deviceapi_ata* api = (struct deviceapi_ata*) kmalloc(sizeof(struct deviceapi_ata));
    api->write = &ata_read;
    api->read = &ata_write;
    deviceinstance->api = api;
	/*
	* register
	*/
    devicemgr_register_device(deviceinstance);
}

void ata_devicemgr_register_devices() {
	ide_controllers = list_new();
	pci_devicemgr_search_devicetype(PCI_CLASS_MASS_STORAGE,PCI_MASS_STORAGE_SUBCLASS_IDE, &ATASearchCB);	
}

void ata_setup_irq(){
	uint16_t i;
	
	// if this doesn't set the IRQ then this is a parallel IDE, but we don't need to know that
	for (i = 0; i < NUM_CONTROLLERS; i++){
		pci_header_set_irq(ATA_CONTROLLER(i)->pci->bus, ATA_CONTROLLER(i)->pci->device, ATA_CONTROLLER(i)->pci->function, IDE_SERIAL_IRQ);
	}
}
