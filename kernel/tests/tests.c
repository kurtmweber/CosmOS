//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/tests.h>

#include <collection/kernelstring/kernelstring.h>
#include <devicemgr/deviceapi/deviceapi_speaker.h>
#include <sound/tone.h>
#include <devicemgr/deviceapi/deviceapi_dsp.h>
#include <devicemgr/deviceapi/deviceapi_serial.h>
#include <notes.h>
#include <devicemgr/devicemgr.h>
#include <devicemgr/deviceapi/deviceapi_rtc.h>
#include <devicemgr/deviceapi/deviceapi_pit.h>
#include <devicemgr/deviceapi/deviceapi_cpu.h>
#include <devicemgr/deviceapi/deviceapi_floppy.h>
#include <mm/mm.h>
#include <debug/debug.h>
#include <string/string.h>
#include <console/console.h>
#include <sleep/sleep.h>

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
	// get the speaker
	struct device* speaker = devicemgr_findDevice("speaker0");
	if (0!=speaker){
		struct deviceapi_speaker* speaker_api = (struct deviceapi_speaker*) speaker->api;
		speaker_beep_function beep_func = speaker_api->beep;
	//	(*beep_func)(speaker, 4000, 50);
		(*beep_func)(speaker,NOTE_G5, 200);
		sleep_wait(100);
		(*beep_func)(speaker,NOTE_G5, 200);
		sleep_wait(100);
		(*beep_func)(speaker,NOTE_G5, 200);
		sleep_wait(100);
		(*beep_func)(speaker,NOTE_DS5, 400);
		sleep_wait(400);

		(*beep_func)(speaker,NOTE_F5, 200);
		sleep_wait(100);
		(*beep_func)(speaker,NOTE_F5, 200);
		sleep_wait(100);
		(*beep_func)(speaker,NOTE_F5, 200);
		sleep_wait(100);
		(*beep_func)(speaker,NOTE_D5, 400);
		sleep_wait(100);
	} else {
		kprintf("Unable to find speaker0\n");
	}
}

void playsb16() {	
	// get the sb
	struct device* dsp = devicemgr_findDevice("dsp0");
	if (0!=dsp) {
		struct wav_header* wav = sound_get_tone();



		struct deviceapi_dsp* dsp_api = (struct deviceapi_dsp*) dsp->api;
		dsp_play_function play_func = dsp_api->play;
		uint8_t* pcmstart =wav_pcm_start(wav);

		(*play_func)(dsp, pcmstart, wav->sample_rate, wav->bit_depth, wav->num_channels, wav->data_bytes);
	} else {
		kprintf("Unable to find dsp0\n");
	}
}

void chirp() {
	// get the speaker
	struct device* speaker = devicemgr_findDevice("speaker0");
	if (0!=speaker){
		struct deviceapi_speaker* speaker_api = (struct deviceapi_speaker*) speaker->api;
		speaker_beep_function beep_func = speaker_api->beep;
		(*beep_func)(speaker, 4000, 50);
	} else {
		kprintf("Unable to find speaker0\n");
	}
}

/*
* write a message via the uniform serial api
*/
void serialMessage(const uint8_t* message) {
   	// get serial0
	struct device* serial0 = devicemgr_findDevice("serial0");
	if (0!=serial0){
		struct deviceapi_serial* serial_api = (struct deviceapi_serial*) serial0->api;
		serial_write_function write_func = serial_api->write;
		(*write_func)(serial0, message);
	} else {
		kprintf("Unable to find serial0\n");
	}
}

void test_debug() {
	uint8_t testdata[] = {0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x11, 0x13, 0x15, 0x17, 0x19, 0x1B };
	debug_show_memblock(testdata,10);
}

void show_cpu_data() {
	/*
	* show CPU features
	*/
	// get the CPU
	struct device* cpu = devicemgr_findDevice("cpu0");
	struct deviceapi_cpu* cpu_api = (struct deviceapi_cpu*) cpu->api;

	/*
	* show all CPU features
	*/
	struct cpu_id id;
	(*cpu_api->features)(&id);
	kprintf("CPU Features %#X\n", id.edx);

	/*
	* check if APIC
	*/
	bool apic = (*cpu_api->apic)();
	if(apic){
		kprintf("APIC present\n");
	} else {
		kprintf("APIC not present\n");
	}
	
	/*
	* show CPU manufacturer
	*/ 
  	uint8_t cpu_manufacturer_string[13];
    (*cpu_api->manufacturer)((uint8_t*)&cpu_manufacturer_string);
    kprintf("CPU Manufacturer: %s\n", cpu_manufacturer_string);
}

void testFunctions() {
	/*
	* exercise the uniform serial API
	*/
	serialMessage("This message brought to you by the uniform serial API, the letters R and S and the Digits 2, 3 and 2\n");

	/*
	* make some noise
	*/
	//chirp();
	//BeethovensFifth();

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

	/*do {
		kprintf("Base: %llX, Length: %llX\n", (uint64_t)tmp->base, tmp->len);
	} while((tmp = tmp->next));

	phys_alloc_slab(65536, 65536);

	tmp = usable_phys_blocks;

	do {
		kprintf("Base: %llX, Length: %llX\n", (uint64_t)tmp->base, tmp->len);
	} while((tmp = tmp->next)); */
	
//	stringtest();
}

void floppyread() {
	// get the floppy
	struct device* floppy = devicemgr_findDevice("floppy0");
	if (0!=floppy){
		struct deviceapi_floppy* floppy_api = (struct deviceapi_floppy*) floppy->api;

		uint8_t data[256];
		(*floppy_api->read)(floppy, 0, data, 255);
		debug_show_memblock(data, 32);
	} else {
		kprintf("Unable to find floppy0\n");
	}
}


