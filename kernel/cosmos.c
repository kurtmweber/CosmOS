/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <system/console/console.h>
#include <system/console/drivers/drivers.h>
#include <system/platform/i386/interrupts/interrupt_router.h>
#include <system/platform/i386/video/video.h>
#include <system/platform/i386/video/vga/vga.h>
#include <system/tests/tests.h>
#include <system/platform/i386/mm/mm.h>
#include <system/devicemgr/devicemgr.h>
#include <asm/asm.h>
#include <system/platform/i386/interrupts/idt.h>
#include <system/tests/testvblock.h>

void CosmOS(){
	video_init();
	video_select_driver(VIDEO_DRIVER_VGA);
	video_select_mode(VIDEO_MODE_TEXT);
	
	console_driver_interface_init();
	/*
	* Put a hello message on the video, just so we know it's ok....
	*/
	console_select_driver(CONSOLE_DRIVER_VGA);
	kprintf("Welcome to CosmOS 0.1\n");
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

	kprintf("Initializing Interrupt Routing...\n");
	interrupt_router_init();

	/*
	* init the device registry
	*/
	kprintf("Initializing Device Registry...\n");
	devicemgr_init();

	/*
	* Register all devices
	*/
	devicemgr_register_devices();
	kprintf("Registered %llu devices\n", devicemgr_device_count());

	/*
	* Init all devices
	*/
	devicemgr_init_devices();
	kprintf("There are %llu devices\n", devicemgr_device_count());
	kprintf("\n");
	kprintf("************************************\n");
	kprintf("** Device Initialization Complete **\n");
	kprintf("************************************\n");
	kprintf("\n");

	/*
	* enable interrupts
	*/
	asm_sti();
	
	/*
	* play
	*/
//	playsb16();
//	floppyread();	

//	test_ata();
	test_vblock();

	while (1){
		asm_hlt();
	}
}


