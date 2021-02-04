//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/smbios/smbios.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_smbios.h>

void test_smbios() {
    kprintf("Testing SMBIOS\n");
    uint8_t devicename[] = {"smbios0"};

    /*
     * find the SMBIOS disk
     */
    struct device* smbios = devicemgr_find_device(devicename);
    if (0 != smbios) {
        ASSERT_NOT_NULL(smbios->deviceData);
        struct sm_bios_entry_point* smbios_entry = smbios_get_sm_bios_entry_point(smbios);

        ASSERT_NOT_NULL(smbios_entry);
        ASSERT(smbios_entry->entry_point_string[0] == '_');
        ASSERT(smbios_entry->entry_point_string[1] == 'S');
        ASSERT(smbios_entry->entry_point_string[2] == 'M');
        ASSERT(smbios_entry->entry_point_string[3] == '_');
        kprintf("table len %llu, table address %#hX\n", smbios_entry->table_length, smbios_entry->table_address);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
