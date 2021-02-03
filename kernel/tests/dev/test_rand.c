//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_rand.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_rand.h>

void test_rand() {
    kprintf("Testing rand\n");

    // get the time, b/c we can
    struct device* rand = devicemgr_find_device("rand0");
    if (0 != rand) {
        struct deviceapi_rand* rand_api = (struct deviceapi_rand*)rand->api;
        for (uint64_t i = 0; i < 100; i++) {
            kprintf("rand: %llu ", (*rand_api->read)(rand));
        }
    } else {
        kprintf("Unable to find rand0\n");
    }
}