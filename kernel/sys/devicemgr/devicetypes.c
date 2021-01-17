//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/console.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicetypes.h>

struct array* types;

void devicetypes_init() {
    kprintf("Init Device Types\n");
    types = array_new(MAX_DEVICE_TYPES);
    for (uint16_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        array_set(types, i, 0);
    }
}

struct arraylist* devicetypes_get_devicelist(deviceType dt) {
    ASSERT_NOT_NULL(types);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        return (struct arraylist*)array_get(types, dt);
    } else {
        panic("Invalid device type passed to devicetypes_get_devicelist");
    }
}

void devicetypes_set_devicelist(deviceType dt, struct arraylist* lst) {
    ASSERT_NOT_NULL(types);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        array_set(types, dt, lst);
    } else {
        panic("Invalid device type passed to devicetypes_set_devicelist");
    }
}
