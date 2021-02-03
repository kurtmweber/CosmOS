//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/acpi/acpi.h>
#include <dev/x86-64/acpi/rsdp.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_bda.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>

uint32_t rsdt_other_sdt_entries(struct acpi_sdt_header* h) {
    return (h->length - sizeof(struct acpi_sdt_header)) / 4;
}

uint32_t xsdt_other_sdt_entries(struct acpi_sdt_header* h) {
    return (h->length - sizeof(struct acpi_sdt_header)) / 8;
}

struct fadt* acpi_get_fadt() {
    struct rsdt* rsdt = rsdp_get_acpi_rsdt();
    ASSERT_NOT_NULL(rsdt);
    debug_show_memblock((uint8_t*)rsdt, 128);
    debug_show_memblock((uint8_t*)&(rsdt->pointerToOtherSDT), 32);

    int entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;
    kprintf("entries %llu\n", entries);

    for (uint8_t i = 0; i < entries; i++) {
        kprintf("i %llu\n", i);

        uint32_t ptr = rsdt->pointerToOtherSDT[i];
        kprintf("hhhjjj\n");
        kprintf("ptr %llu\n", ptr);

        struct acpi_sdt_header* h = (struct acpi_sdt_header*)CONV_PHYS_ADDR((uint64_t)ptr);
        if (!strncmp(h->signature, "FACP", 4)) {
            return (void*)h;
        }
    }
    // No fadt found
    return 0;
}

/*
 * perform device instance specific init here
 */
uint8_t acpi_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    //  uint64_t ebda_address = bda_get_ebda_address();

    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

void acpi_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "ACPI");
    deviceinstance->devicetype = ACPI;
    deviceinstance->init = &acpi_device_init;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}