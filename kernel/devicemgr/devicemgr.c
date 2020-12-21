//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <collection/list/list.h>
#include <mm/mm.h>
#include <string/string.h>
#include <panic/panic.h>
#include <devicemgr/deviceregistry.h>

#define MAX_DEVICE_NAME_LENGTH 64

int8_t* DeviceTypeNames[] = {"None"
	, "serial"
	,"vga"
	,"rtc"
	,"keyboard"
	,"ethernet"
	,"bridge"
	,"usb"
	,"ata"
	,"pic"
	,"mouse"
	,"floppy"
	,"speaker"
	,"pit"
	,"dsp"
    }; 

void devicemgr_init() {
    deviceregistry_init();    
}

int8_t* createDeviceName(struct device* dev) {
    int8_t nn[32];
    int8_t* ret = kmalloc(MAX_DEVICE_NAME_LENGTH);
    strcpy(ret, DeviceTypeNames[dev->devicetype]);
    uitoa3(dev->type_index, nn, 32, 10);
    ret = strcat(ret, nn);
  //  kprintf(ret);
    return ret;
}

void devicemgr_register_device(struct device* dev) {
    if (0==dev){
        panic("Attempt to register null device\n");
    }
    if (0==dev->description){
        panic("Attempt to register device without description\n");
    }
    if (0==dev->devicetype){
        kprintf(dev->description);
        panic("Attempt to register device without deviceType\n");
    }
    if (0==dev->init){
        kprintf(dev->description);
        panic("Attempt to register device without init function\n");
    }
    /*
    * set index
    */
   dev->type_index = deviceregistry_devicecount_type(dev->devicetype);
    /*
    * create name
    */
    dev->name = createDeviceName(dev);
    /*
    * register
    */
    deviceregistry_registerdevice(dev);
}

uint16_t devicemgr_device_count() {
    return deviceregistry_devicecount();
}

void deviceInitIterator(struct device* dev) {
    if (0!=dev) {
        dev->init(dev);
    } else {
        panic("um. why is there a null device?");
    }
}

/*
* init order matters
*/
void devicemgr_init_devices(){
    kprintf("Initializing Devices\n");
    /*
    * PIC first
    */
    deviceregistry_iterate_type(PIC, deviceInitIterator);
    /*
    * Serial next
    */
    deviceregistry_iterate_type(SERIAL, deviceInitIterator);
   /*
    * the PIT
    */
    deviceregistry_iterate_type(PIT, deviceInitIterator);
    /*
    * everything else
    */
    deviceregistry_iterate_type(RTC, deviceInitIterator);
    deviceregistry_iterate_type(KEYBOARD, deviceInitIterator); 
    deviceregistry_iterate_type(VGA, deviceInitIterator);
    deviceregistry_iterate_type(USB, deviceInitIterator);
    deviceregistry_iterate_type(ETHERNET, deviceInitIterator);
    deviceregistry_iterate_type(BRIDGE, deviceInitIterator);
    deviceregistry_iterate_type(ATA, deviceInitIterator);
    deviceregistry_iterate_type(MOUSE, deviceInitIterator);
    deviceregistry_iterate_type(FLOPPY, deviceInitIterator);
    deviceregistry_iterate_type(SPEAKER, deviceInitIterator);
    deviceregistry_iterate_type(DSP, deviceInitIterator);
}

struct device* devicemgr_new_device() {
    struct device* ret= (struct device*) kmalloc(sizeof(struct device));
    ret->description=0;
    ret->init=0;
    ret->deviceData=0;
    ret->name=0;
    ret->type_index=0;
    ret->devicetype=0;
    ret->api=0;
    return ret;
}

void devicemgr_set_device_description(struct device* dev, int8_t* description) {
    if ((0!=description) && (0!=dev)){
        uint32_t size = strlen(description);
        if (0!=dev->description){
            kfree(dev->description);
        }
        dev->description = kmalloc(size+1);
        strcpy(dev->description, description);
    } else {
        panic("Invalid device or description passed to devicemgr_set_device_description\n");
    }
}

struct device* devicemgr_findDevice(const int8_t* name) {
    if (0!=name){
        return deviceregistry_findDevice(name);
    } else {
        panic("Invalid device name passed to devicemgr_findDevice\n");
    }
}

