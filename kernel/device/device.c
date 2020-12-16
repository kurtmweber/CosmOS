//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/device.h>
#include <console/console.h>
#include <array/array.h>

#define MAX_DEVICES 64

struct array* devices;
uint16_t device_index = 0;

void device_registry_init() {
    devices = arrayNew(MAX_DEVICES);    
}

void registerDevice(struct device* dev) {
    kprintf("register");
    arraySet(devices,device_index++, dev );
}

uint16_t deviceCount() {
    return device_index;
}

struct device* getDevice(uint16_t idx){
    return arrayGet(devices, idx);
}

void initDevices(){
    for (uint16_t i=0; i<=device_index;i++){
        struct device* dev = (struct device*) arrayGet(devices, i);
        dev->init();
    }
}
