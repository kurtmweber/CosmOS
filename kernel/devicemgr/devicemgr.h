//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEMGR_H
#define _DEVICEMGR_H

#include <types.h>

struct device;

typedef void (*deviceInit)(struct device* dev);

typedef enum deviceType {
	NONE	=		0x00,
	SERIAL	=		0x01,
	VGA = 			0x02,
	RTC=			0x03,
	KEYBOARD=		0x04,
	ETHERNET=		0x05,
	BRIDGE=			0x06,
	USB=			0x07,
	ATA=			0x08,
	PIC=			0x09,
	MOUSE=			0x0A,
	FLOPPY=			0x0B,
	SPEAKER=		0x0C,
	PIT=			0x0D
} deviceType;

typedef struct device {
	int8_t* name;
	enum deviceType devicetype;
	deviceInit init;
	int8_t* description;
	void* deviceData;
} device_t;

// new device, allocated on the kernel heap
struct device* devicemgr_new_device();

// register a device
void devicemgr_register_device(struct device* dev); 

// init the device registry
void devicemgr_init();

// count of device instances
uint16_t devicemgr_device_count();

// get a device instance
struct device* devicemgr_get_device(uint16_t idx);

// init all devices
void devicemgr_init_devices();

// set description
void devicemgr_set_device_description(struct device* dev, int8_t* description);

// search for device by device type
typedef void (*deviceSearchCallback)(struct device* dev);
void devicemgr_search_device(enum deviceType devicetype, deviceSearchCallback cb);

#endif