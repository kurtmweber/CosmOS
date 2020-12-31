//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testpit.h>
#include <debug/debug.h>
#include <devicemgr/deviceapi/deviceapi_pit.h>
#include <console/console.h>
#include <sleep/sleep.h>

void test_pit() {
	// get the PIT
	struct device* pit = devicemgr_findDevice("pit0");
    if (0!=pit){
        struct deviceapi_pit* pit_api = (struct deviceapi_pit*) pit->api;
        pit_tickcount_function tickcount_func = pit_api->tickcount;

        // show the tick count, since we can
        uint64_t tc = (*tickcount_func)(pit);
        kprintf("Ticks: %llu\n", tc);
        sleep_wait(1000);
        tc = (*tickcount_func)(pit);
        kprintf("Ticks: %llu\n", tc);
    } else {
        kprintf("Unable to find pit0\n");
    }
}