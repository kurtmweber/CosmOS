//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/cpu/cpu.h>
#include <sys/asm/asm.h>
#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_cpu.h>
#include <sys/devicemgr/devicemgr.h>

void cpu_manufacturer_id(uint8_t* cpuid);

/*
 * perform device instance specific init here
 */
void cpu_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
}

// https://prodevsblog.com/questions/441449/checking-if-cpu-supports-rdrand/
void invokeCPUID(unsigned int function, unsigned int subfunction, unsigned int* pEAX, unsigned int* pEBX, unsigned int* pECX, unsigned int* pEDX) {
    asm volatile("cpuid" : "=a"(*pEAX), "=b"(*pEBX), "=c"(*pECX), "=d"(*pEDX) : "a"(function), "c"(subfunction));
}

void cpu_get_features(struct cpu_id* id) {
    ASSERT_NOT_NULL(id);

    invokeCPUID(1, 0, &(id->eax), &(id->ebx), &(id->ecx), &(id->edx));
}

/*
 * https://en.wikipedia.org/wiki/CPUID
 */
bool cpu_has_apic() {
    struct cpu_id id;
    cpu_get_features(&id);
    return ((id.edx & CPUID_FEAT_EDX_APIC) > 0);
}

void cpu_manufacturer_id(uint8_t* cpuid) {
    ASSERT_NOT_NULL(cpuid);
    struct cpu_id id;
    invokeCPUID(0, 0, &(id.eax), &(id.ebx), &(id.ecx), &(id.edx));

    cpuid[3] = (id.ebx & 0xFF000000) >> 24;
    cpuid[2] = (id.ebx & 0x00FF0000) >> 16;
    cpuid[1] = (id.ebx & 0x0000FF00) >> 8;
    cpuid[0] = (id.ebx & 0x000000FF) >> 0;
    cpuid[7] = (id.edx & 0xFF000000) >> 24;
    cpuid[6] = (id.edx & 0x00FF0000) >> 16;
    cpuid[5] = (id.edx & 0x0000FF00) >> 8;
    cpuid[4] = (id.edx & 0x000000FF) >> 0;
    cpuid[11] = (id.ecx & 0xFF000000) >> 24;
    cpuid[10] = (id.ecx & 0x00FF0000) >> 16;
    cpuid[9] = (id.ecx & 0x0000FF00) >> 8;
    cpuid[8] = (id.ecx & 0x000000FF) >> 0;
    cpuid[12] = 0;
}

void cpu_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "CPU");
    deviceinstance->devicetype = CPU;
    deviceinstance->init = &cpu_device_init;
    /*
     * device api
     */
    struct deviceapi_cpu* api = (struct deviceapi_cpu*)kmalloc(sizeof(struct deviceapi_cpu));
    api->features = &cpu_get_features;
    api->apic = &cpu_has_apic;
    api->manufacturer = &cpu_manufacturer_id;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}