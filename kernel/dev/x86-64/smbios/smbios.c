//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/System_Management_BIOS

#include <dev/x86-64/smbios/smbios.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_bda.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>

struct smbios_devicedata {
    uint64_t base;
};

uint64_t smbios_find() {
    uint8_t* mem = (uint8_t*)0xF0000;
    uint64_t length, i;
    uint8_t checksum;
    while ((uint64_t)mem < 0x100000) {
        if (mem[0] == '_' && mem[1] == 'S' && mem[2] == 'M' && mem[3] == '_') {
            length = mem[5];
            checksum = 0;
            for (i = 0; i < length; i++) {
                checksum += mem[i];
            }
            if (checksum == 0)
                break;
        }
        mem += 16;
    }
    if ((uint64_t)mem == 0x100000) {
        return 0;
    } else {
        return (uint64_t)mem;
    }
}

struct sm_bios_entry_point* smbios_get_sm_bios_entry_point(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct smbios_devicedata* deviceData = (struct smbios_devicedata*)dev->deviceData;
    return (struct sm_bios_entry_point*)deviceData->base;
}

/*
 * perform device instance specific init here
 */
uint8_t smbios_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct smbios_devicedata* deviceData = (struct smbios_devicedata*)dev->deviceData;
    deviceData->base = smbios_find();
    if (0 != deviceData->base) {
        kprintf("Init %s (%s) at Base %#hX\n", dev->description, dev->name, deviceData->base);
        return 1;
    } else {
        // cant find the mem region, we cant init it
        return 0;
    }
}

void smbios_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "SMBIOS");
    deviceinstance->devicetype = SMBIOS;
    deviceinstance->init = &smbios_device_init;
    /*
     * device data
     */
    struct smbios_devicedata* deviceData = (struct smbios_devicedata*)kmalloc(sizeof(struct smbios_devicedata));
    deviceData->base = 0;
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}