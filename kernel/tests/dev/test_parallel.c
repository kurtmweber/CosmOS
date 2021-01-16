//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/deviceapi/deviceapi_parallel.h>
#include <tests/dev/test_parallel.h>

void test_parallel() {
    uint8_t devicename[] = {"par0"};

    struct device* parallel = devicemgr_find_device(devicename);
    if (0 != parallel) {
        struct deviceapi_parallel* parallel_api = (struct deviceapi_parallel*)parallel->api;

        (*parallel_api->write)(parallel, "hello", 6);

    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
