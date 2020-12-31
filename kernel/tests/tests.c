//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/tests.h>
#include <devicemgr/devicemgr.h>
#include <devicemgr/deviceapi/deviceapi_rtc.h>
#include <devicemgr/deviceapi/deviceapi_pit.h>
#include <mm/mm.h>
#include <debug/debug.h>
#include <console/console.h>
#include <sleep/sleep.h>



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



