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
#include <timing/timing.h>
#include <video/video.h>
#include <video/vga/vga.h>
#include <serial/serial.h>
#include <usb/usb.h>
#include <device/device.h>
#include <network/network.h>
#include <bridge/bridge.h>

void CosmOS(){	
	video_init();
	video_select_driver(VIDEO_DRIVER_VGA);
	video_select_mode(VIDEO_MODE_TEXT);
	
	console_driver_interface_init();
	console_select_driver(CONSOLE_DRIVER_VGA);
	
	kprintf("Loading CosmOS 0.1\n");
	
	kprintf("Initializing IDT...\n");
	initIDT();
	
	kprintf("Initializing MMU...\n");
	mmu_init();
	
	kprintf("Initializing PIC...\n");
	pic_init();

	kprintf("Initializing Interrupt Routing...\n");
	interrupt_router_init();

	serial_init();
	serial_write("Hello Serial 1\n");	

	rtc_init();
	keyboard_init();
	
	pci_init();

	usb_init();
	network_init();
	bridge_init();
	ata_init();
	
	/*
	* init all devices
	*/
//	initDevices();

	asm_sti();

	mem_block *tmp;
	tmp = usable_phys_blocks;

	do {
		kprintf("Base: %llX, Length: %llX\n", (uint64_t)tmp->base, tmp->len);
	} while((tmp = tmp->next));

	phys_alloc_slab(65536, 65536);

	tmp = usable_phys_blocks;

	do {
		kprintf("Base: %llX, Length: %llX\n", (uint64_t)tmp->base, tmp->len);
	} while((tmp = tmp->next));
	
	while (1){
		asm_hlt();
	}
}

#undef P