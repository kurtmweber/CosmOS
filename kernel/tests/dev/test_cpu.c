//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/deviceapi/deviceapi_cpu.h>
#include <tests/dev/test_cpu.h>

void show_cpu_data() {
    // get the CPU
    struct device* cpu = devicemgr_find_device("cpu0");
    if (0 != cpu) {
        struct deviceapi_cpu* cpu_api = (struct deviceapi_cpu*)cpu->api;
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
        if (apic) {
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
    } else {
        kprintf("Unable to find cpu0\n");
    }
}
