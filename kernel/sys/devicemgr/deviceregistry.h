//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEREGISTRY_H
#define _DEVICEREGISTRY_H

#include <sys/devicemgr/devicemgr.h>

void deviceregistry_init();
void deviceregistry_registerdevice(struct device* dev);
uint16_t deviceregistry_devicecount();
uint16_t deviceregistry_devicecount_type(deviceType dt);
struct device* deviceregistry_get_device(deviceType dt, uint16_t idx);

typedef void (*DeviceIterator)(struct device* dev);

void deviceregistry_iterate(DeviceIterator deviceIterator);
void deviceregistry_iterate_type(deviceType dt, DeviceIterator deviceIterator);

// find a device by name
struct device* deviceregistry_findDevice(const int8_t* name);

#endif