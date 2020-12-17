//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICE_H
#define _DEVICE_H

#include <types.h>

struct device;

typedef void (*deviceInit)(struct device* dev);

typedef struct device {
	int8_t* name;
	deviceInit init;
	int8_t* description;
	void* deviceData;
} device_t;

// new device, allocated on the kernel heap
struct device* newDevice();

// register a device
void registerDevice(struct device* dev); 

// init the device registry
void device_registry_init();

// count of device instances
uint16_t deviceCount();

// get a device instance
struct device* getDevice(uint16_t idx);

// init all devices
void initDevices();

// set description
void deviceSetDescription(struct device* dev, int8_t* description);

#endif