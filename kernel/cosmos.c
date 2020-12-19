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
#include <dev/ata/ata.h>
#include <interrupts/interrupts.h>
#include <interrupts/interrupt_router.h>
#include <dev/keyboard/keyboard.h>
#include <mm/mm.h>
#include <dev/pci/pci.h>
#include <string/string.h>
#include <dev/rtc/rtc.h>
#include <video/video.h>
#include <video/vga/vga.h>
#include <dev/serial/serial.h>
#include <dev/usb/usb.h>
#include <devicemgr/devicemgr.h>
#include <dev/network/network.h>
#include <dev/bridge/bridge.h>
#include <dev/display/display.h>
#include <collection/kernelstring/kernelstring.h>

void stringtest();

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
	* get the PCI bus info
	*/
	pci_init();

	/*
	* register all devices.  this just makes a list of device instances
	*/
	kprintf("Initializing Device Registry...\n");
	device_registry_init();

	/*
	* devices. Note that devices are initialized in the order they are registered.  So PIC first....
	*/
	pic_register_devices();
	serial_register_devices();
	rtc_register_devices();
	keyboard_register_devices();
	display_register_devices();
	usb_register_devices();
	network_register_devices();
	bridge_register_devices();
	ata_register_devices();
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
	
//	stringtest();

	while (1){
		asm_hlt();
	}
}

void stringtest() {
	// concat
	struct kernelstring* string1 = stringFromCStr("hello");
	struct kernelstring* string2 = stringFromCStr(" world\n");

	kprintf("%llu\n",string1->length);
	kprintf(stringGetCStr(string1));
	kprintf("%llu\n",string2->length);
	kprintf(stringGetCStr(string2));

	struct kernelstring* string3 = stringConcat(string1, string2);
	kprintf("%llu\n",string3->length);
	kprintf(stringGetCStr(string3));

	struct kernelstring* string4 = stringItoa3(1000, 16);
	kprintf("%llu\n",string4->length);
	kprintf(stringGetCStr(string4));

	struct kernelstring* string5 = stringCopy(string3);
	kprintf("%llu\n",string3->length);
	kprintf(stringGetCStr(string3));
	kprintf("\n");

	struct kernelstring* string6 = stringCopy(string3);
	kprintf("%llu\n",string3->length);
	kprintf(stringGetCStr(string3));
	kprintf("\n");

	struct kernelstring* string7 = stringFromCStr("beer           ");
	kprintf("%llu\n",string7->length);
	kprintf(stringGetCStr(string7));
	kprintf("\n");

	struct kernelstring* string8 = stringTrim(string7);
	kprintf("%llu\n",string8->length);
	kprintf(stringGetCStr(string8));
	kprintf("\n");
}
#undef P