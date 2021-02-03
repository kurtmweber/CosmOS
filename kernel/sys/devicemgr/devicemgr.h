//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEMGR_H
#define _DEVICEMGR_H

#include <sys/devicemgr/device.h>
#include <types.h>

// new device, allocated on the kernel heap
struct device* devicemgr_new_device();

// register a device
void devicemgr_register_device(struct device* dev);

// unregister a device
void devicemgr_unregister_device(struct device* dev);

// init the device registry
void devicemgr_init();

// register all the devices
void devicemgr_register_devices();

// count of device instances
uint16_t devicemgr_device_count();

// init all devices
void devicemgr_init_devices();

// set description
void devicemgr_set_device_description(struct device* dev, const uint8_t* description);

// find a device ie ("rtc0")
struct device* devicemgr_find_device(const uint8_t* name);

// find devices by the device description
typedef void (*deviceSearchCallback)(struct device* dev);
void devicemgr_find_devices_by_description(deviceType dt, const uint8_t* description, deviceSearchCallback cb);

// find devices by deviceType
void devicemgr_find_devices_by_deviceType(deviceType dt, deviceSearchCallback cb);

// attach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t devicemgr_attach_device(struct device* dev);

// detach a device (non-fixed devices... like RAM disks and SWAP)
// be aware, this has the effect of deleting the dev struct!
uint8_t devicemgr_detach_device(struct device* dev);

#endif