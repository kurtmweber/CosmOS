//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/bda/bda.h>
#include <dev/i386/ebda/acpi.h>
#include <dev/i386/ebda/ebda.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <tests/dev/test_bda.h>

void test_bda() {
    struct rsdp_descriptor_2* rsdp = ebda_get_rsdp();
    // debug_show_memblock(rsdp, 32);

    struct acpi_sdt_header* acpi = rsdp_get_acpi_header(rsdp);
    kprintf("ACPI %#X\n", acpi);
    // debug_show_memblock(acpi, 32);
}
