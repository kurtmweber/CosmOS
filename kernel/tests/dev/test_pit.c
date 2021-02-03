//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_pit.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <tests/dev/test_pit.h>

void test_pit() {
    // get the PIT
    struct device* pit = devicemgr_find_device("pit0");
    if (0 != pit) {
        struct deviceapi_pit* pit_api = (struct deviceapi_pit*)pit->api;
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