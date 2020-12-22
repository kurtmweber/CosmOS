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
#include <interrupts/interrupts.h>
#include <interrupts/interrupt_router.h>
#include <mm/mm.h>
#include <string/string.h>
#include <video/video.h>
#include <video/vga/vga.h>
#include <collection/kernelstring/kernelstring.h>
#include <dev/dev.h>
#include <sleep/sleep.h>
#include <notes.h>
#include <devicemgr/deviceapi/deviceapi_rtc.h>
#include <devicemgr/deviceapi/deviceapi_speaker.h>
#include <devicemgr/deviceapi/deviceapi_pit.h>

void stringtest();
void BeethovensFifth();

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
	* Register devices
	*/
	dev_devicemgr_register_devices();
	kprintf("Registered %llu devices\n", devicemgr_device_count());

	/*
	* init all devices
	*/
	devicemgr_init_devices();
	kprintf("There are %llu devices\n", devicemgr_device_count());

	asm_sti();

	// get the speaker
	struct device* speaker = devicemgr_findDevice("speaker0");
	struct deviceapi_speaker* speaker_api = (struct deviceapi_speaker*) speaker->api;
	speaker_beep_function beep_func = speaker_api->beep;
	(*beep_func)(speaker, 4000, 50);

//	BeethovensFifth();

	// get the PIT
	struct device* pit = devicemgr_findDevice("pit0");
	struct deviceapi_pit* pit_api = (struct deviceapi_pit*) pit->api;
	pit_tickcount_function tickcount_func = pit_api->tickcount;

	// show the tick count, since we can
	uint64_t tc = (*tickcount_func)(pit);
	kprintf("Ticks: %llu\n", tc);
	sleep_wait(1000);
	tc = (*tickcount_func)(pit);
	kprintf("Ticks: %llu\n", tc);

	// get the time, b/c we can
	struct device* rtc = devicemgr_findDevice("rtc0");
	struct deviceapi_rtc* rtc_api = (struct deviceapi_rtc*) rtc->api;
	rtc_time_function time_func = rtc_api->rtc_time;
	rtc_time_t daTime = (*time_func)(rtc);
	kprintf("Hour: %llu Minute: %llu Second: %llu\n",daTime.hour, daTime.minute, daTime.second);

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
	struct kernelstring* string1 = string_from_cstr("hello");
	struct kernelstring* string2 = string_from_cstr(" world\n");

	kprintf("%llu\n",string1->length);
	kprintf(string_get_cstr(string1));
	kprintf("%llu\n",string2->length);
	kprintf(string_get_cstr(string2));

	struct kernelstring* string3 = string_concat(string1, string2);
	kprintf("%llu\n",string3->length);
	kprintf(string_get_cstr(string3));

	struct kernelstring* string4 = string_itoa3(1000, 16);
	kprintf("%llu\n",string4->length);
	kprintf(string_get_cstr(string4));

	struct kernelstring* string5 = string_copy(string3);
	kprintf("%llu\n",string3->length);
	kprintf(string_get_cstr(string3));
	kprintf("\n");

	struct kernelstring* string6 = string_copy(string3);
	kprintf("%llu\n",string3->length);
	kprintf(string_get_cstr(string3));
	kprintf("\n");

	struct kernelstring* string7 = string_from_cstr("beer           ");
	kprintf("%llu\n",string7->length);
	kprintf(string_get_cstr(string7));
	kprintf("\n");

	struct kernelstring* string8 = string_trim(string7);
	kprintf("%llu\n",string8->length);
	kprintf(string_get_cstr(string8));
	kprintf("\n");
}

void BeethovensFifth() {
//	speaker_beep(NOTE_G5, 200);
	sleep_wait(100);
//	speaker_beep(NOTE_G5, 200);
	sleep_wait(100);
//	speaker_beep(NOTE_G5, 200);
	sleep_wait(100);
//	speaker_beep(NOTE_DS5, 400);
	sleep_wait(400);

//	speaker_beep(NOTE_F5, 200);
	sleep_wait(100);
//	speaker_beep(NOTE_F5, 200);
	sleep_wait(100);
//	speaker_beep(NOTE_F5, 200);
	sleep_wait(100);
//	speaker_beep(NOTE_D5, 400);
	sleep_wait(100);
}

