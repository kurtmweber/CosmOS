/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <console/console.h>
#include <console/drivers/drivers.h>
#include <asm/asm.h>
#include <ata/ata.h>
#include <interrupts/interrupts.h>
#include <interrupts/interrupt_router.h>
#include <keyboard/keyboard.h>
#include <mm/mm.h>
#include <pci/pci.h>
#include <string/string.h>
#include <rtc/rtc.h>
#include <video/video.h>
#include <video/vga/vga.h>
#include <serial/serial.h>
#include <usb/usb.h>
#include <devicemgr/devicemgr.h>
#include <network/network.h>
#include <bridge/bridge.h>
#include <display/display.h>

void CosmOS(){
	video_init();
	video_select_driver(VIDEO_DRIVER_VGA);
	video_select_mode(VIDEO_MODE_TEXT);
	
	console_driver_interface_init();
	/*
	* Put a hello message on the video, just so we know it's ok....
	*/
	console_select_driver(CONSOLE_DRIVER_VGA);
	kprintf("Welcome CosmOS 0.1\n");
	/*
	* ok, output to the serial console now
	*/
	console_select_driver(CONSOLE_DRIVER_SERIAL);
	
	/*
	* Hello!
	*/
	kprintf("Loading CosmOS 0.1\n");
	
	kprintf("Initializing IDT...\n");
	idt_init();
	
	kprintf("Initializing MMU...\n");
	mmu_init();
	
	kprintf("Initializing PIC...\n");
	pic_init();

	kprintf("Initializing Interrupt Routing...\n");
	interrupt_router_init();

	/*
	* get the PCI bus info
	*/
	pci_init();
	/*
	* register all devices.  this just makes a list of device instances
	*/
	kprintf("Initializing Device Registry...\n");
	device_registry_init();
	/*
	* devices
	*/
	serial_register_devices();
	rtc_register_devices();
	keyboard_register_devices();
	display_register_devices();
	usb_register_devices();
	network_register_devices();
	bridge_register_devices();
	ata_init();
	
	/*
	* init all devices
	*/
	initDevices();
	kprintf("There are %llu devices\n", deviceCount());

	asm_sti();

	mem_block *tmp;
	tmp = usable_phys_blocks;

	do {
//		kprintf("Base: %llX, Length: %llX\n", (uint64_t)tmp->base, tmp->len);
	} while((tmp = tmp->next));

	phys_alloc_slab(65536, 65536);

	tmp = usable_phys_blocks;

	do {
//		kprintf("Base: %llX, Length: %llX\n", (uint64_t)tmp->base, tmp->len);
	} while((tmp = tmp->next));
	
	while (1){
		asm_hlt();
	}
}

#undef P