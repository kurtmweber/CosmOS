//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/cpu/cpu.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <devicemgr/deviceapi/deviceapi_cpu.h>

/*
* perform device instance specific init here
*/

void deviceInitCPU(struct device* dev){
    kprintf("Init %s (%s)\n",dev->description, dev->name);
}

// https://prodevsblog.com/questions/441449/checking-if-cpu-supports-rdrand/
void invokeCPUID(unsigned int  function, unsigned int  subfunction, unsigned int* pEAX, unsigned int* pEBX, unsigned int* pECX, unsigned int* pEDX) {
    asm volatile("cpuid"
        :  "=a" (*pEAX),
            "=b" (*pEBX),
            "=c" (*pECX),
            "=d" (*pEDX)
        :   "a" (function),
            "c" (subfunction)
        );
}

void cpu_get_features(struct cpu_id* id) {    
    invokeCPUID(1,0, &(id->eax),&(id->ebx),&(id->ecx),&(id->edx));
}

/* 
* https://en.wikipedia.org/wiki/CPUID
*/
bool has_apic_function() {
    struct cpu_id id;
    cpu_get_features(&id);
    return ((id.edx & CPUID_FEAT_EDX_APIC)>0);
}

void cpu_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "CPU");
	deviceinstance->devicetype = CPU;
	deviceinstance->init =  &deviceInitCPU;
	/*
	* device api
	*/
	struct deviceapi_cpu* api = (struct deviceapi_cpu*) kmalloc (sizeof(struct deviceapi_cpu));
	api->features = &cpu_get_features;
    api->apic = &has_apic_function;
	deviceinstance->api = api;
    /*
    * register
    */ 
	devicemgr_register_device(deviceinstance);
}