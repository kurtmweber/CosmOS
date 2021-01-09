/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/console/console.h>
#include <sys/console/drivers/drivers.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/i386/video/video.h>
#include <sys/i386/video/vga/vga.h>
#include <tests/tests.h>
#include <sys/i386/mm/mm.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/asm/asm.h>
#include <sys/i386/interrupts/idt.h>
#include <sys/deviceapi/deviceapi_rtc.h>
#include <sys/deviceapi/deviceapi_speaker.h>
#include <sys/deviceapi/deviceapi_pit.h>
#include <sys/deviceapi/deviceapi_serial.h>
#include <sys/deviceapi/deviceapi_cpu.h>
#include <sys/deviceapi/deviceapi_dsp.h>
#include <sys/sync/sync.h>

// testing slab allocator
#include <sys/i386/mm/pagetables.h>
// end slab allocator test includes

void stringtest();
void BeethovensFifth();
void chirp();
void serialMessage(const uint8_t* message);
void testFunctions();
void show_cpu_data();
void playsb16();

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

	kprintf("Initializing spinlocks...\n");
	spinlocks_init();

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

	asm_sti();

	show_cpu_data();
//	playsb16();
	/*
	* run various functions to show that things work....
	*/
	//testFunctions();


	while (1){
		asm_hlt();
	}
}

void show_cpu_data() {
	/*
	* show CPU features
	*/
	// get the CPU
	struct device* cpu = devicemgr_find_device("cpu0");
	struct deviceapi_cpu* cpu_api = (struct deviceapi_cpu*) cpu->api;

	/*
	* show all CPU features
	*/
	struct cpu_id id;
	(*cpu_api->features)(&id);
	kprintf("CPU Features %#X\n", id.edx);

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
//	test_vblock();
//	test_ata();
//	test_ramdisk();
	test_cfs();
//test_fat();
	while (1){
		asm_hlt();
	}
}


