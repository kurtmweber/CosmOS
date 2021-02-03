//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/acpi/acpi.h>
#include <dev/x86-64/bda/bda.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_bda.h>

void test_acpi() {
    kprintf("Testing ACPI\n");
    struct fadt* fadt = acpi_get_fadt();
    ASSERT_NOT_NULL(fadt);
    debug_show_memblock((uint8_t*)fadt, 32);
}
