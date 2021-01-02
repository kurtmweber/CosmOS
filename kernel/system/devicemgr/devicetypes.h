//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICETYPES_H
#define _DEVICETYPES_H

#include <collection/array/array.h>
#include <collection/list/list.h>
#include <system/devicemgr/devicemgr.h>

#define MAX_DEVICE_TYPES 64

void devicetypes_init();
struct list* devicetypes_get_devicelist(deviceType dt);
void devicetypes_set_devicelist(deviceType dt, struct list* lst);

#endif