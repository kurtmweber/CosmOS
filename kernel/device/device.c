//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/device.h>
#include <console/console.h>


#define MAX_DEVICES 64

struct device* devices[MAX_DEVICES];
uint16_t device_index = 0;

void device_registry_init() {
    for (uint16_t i=0; i<MAX_DEVICES;i++){
        devices[i]=0;
    }
}

void registerDevice(struct device* dev) {
    kprintf("register");
    devices[device_index++]=dev;
}

uint16_t deviceCount() {
    return device_index;
}

struct device* getDevice(uint16_t idx){
    return devices[idx];
}

void initDevices(){
    for (uint16_t i=0; i<=device_index;i++){
        devices[i]->init();
    }
}
