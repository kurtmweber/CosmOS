//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICETYPE_SERIAL_H
#define _DEVICETYPE_SERIAL_H

#include <types.h>

typedef void (*DeviceTypeSerial_write)(struct device* dev, const uint8_t* c);

struct DeviceType_serial {
    DeviceTypeSerial_write deviceTypeSerial_write;
};

#endif