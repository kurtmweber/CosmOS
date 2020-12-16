//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICE_H
#define _DEVICE_H

#include <types.h>

typedef void (*deviceInit)();

typedef struct device {
	deviceInit init;
	void* deviceData;
} device_t;

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

#endif