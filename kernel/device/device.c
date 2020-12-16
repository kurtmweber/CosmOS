//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/device.h>
#include <console/console.h>
#include <array/array.h>
#include <mm/mm.h>
#include <string/string.h>

#define MAX_DEVICES 64

struct array* devices;
uint16_t device_index = 0;

void device_registry_init() {
    devices = arrayNew(MAX_DEVICES);    
}

void registerDevice(struct device* dev) {
    arraySet(devices,device_index++, dev );
}

uint16_t deviceCount() {
    return device_index;
}

struct device* getDevice(uint16_t idx){
    return arrayGet(devices, idx);
}

void initDevices(){
    kprintf("Initializing Devices\n");
    for (uint16_t i=0; i<=device_index;i++){
        struct device* dev = (struct device*) arrayGet(devices, i);
        if (0!=dev){
            dev->init();
        }
    }
}

struct device* newDevice() {
    struct device* ret= (struct device*) kmalloc(sizeof(struct device));
    ret->description=0;
    ret->init=0;
    ret->deviceData=0;
    return ret;
}

/*
void deviceSetDescription(struct device* dev, int8_t* description) {
    if ((0!=description) && (0!=dev)){
        uint32_t size = strlen(description);
        if (0!=dev->description){
            kfree(dev->description);
        }
        dev->description = kmalloc(size+1);
        strcpy(description, dev->description);
    }
}
*/


