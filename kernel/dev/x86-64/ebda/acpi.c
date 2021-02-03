//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/ebda/acpi.h>
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

struct fadt* acpi_get_fadt(struct rsdt* rsdt) {
    ASSERT_NOT_NULL(rsdt);
    int entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;
    kprintf("entries %llu\n", entries);

    for (uint64_t i = 0; i < entries; i++) {
        kprintf("i %llu\n", i);

        uint32_t ptr = rsdt->pointerToOtherSDT[i];
        kprintf("ptr %llu\n", ptr);

        struct acpi_sdt_header* h = (struct acpi_sdt_header*)CONV_PHYS_ADDR((uint64_t)ptr);
        if (!strncmp(h->signature, "FADT", 4))
            return (void*)h;
    }
    // No fadt found
    return 0;
}
