//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/acpi/ebda.h>
#include <dev/x86-64/bda/bda.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_bda.h>

void test_bda() {
    kprintf("Testing BDA\n");

    struct rsdp_descriptor_2* rsdp = ebda_get_rsdp();
    ASSERT_NOT_NULL(rsdp);

    // debug_show_memblock((uint8_t*)rsdp, 32);

    uint8_t oem[255];
    rsdp_get_oem_id(rsdp, oem);
    //  kprintf("oem %s\n", oem);

    uint8_t sig[255];
    rsdp_get_signature(rsdp, sig);
    //   kprintf("sig %s\n", sig);
    //   kprintf("version %#llX\n", rsdp_get_acpi_version(rsdp));

    struct rsdt* rsdt = rsdp_get_acpi_rsdt(rsdp);
    ASSERT_NOT_NULL(rsdt);
    // kprintf("RSDT %#llX\n", rsdt);
    //     debug_show_memblock((uint8_t*)rsdt, 128);
    //    debug_show_memblock((uint8_t*)&(rsdt->pointerToOtherSDT), 32);
}
