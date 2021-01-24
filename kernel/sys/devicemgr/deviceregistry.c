//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/deviceregistry.h>
#include <sys/devicemgr/devicetypes.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>

void deviceregistry_init() {
    //   kprintf("Init Device Registry\n");
    devicetypes_init();
}

void deviceregistry_registerdevice(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->devicetype);

    struct arraylist* lst = devicetypes_get_devicelist(dev->devicetype);
    if (0 == lst) {
        lst = arraylist_new();
        devicetypes_set_devicelist(dev->devicetype, lst);
    }
    arraylist_add(lst, dev);
}

void deviceregistry_unregisterdevice(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->devicetype);
    struct arraylist* lst = devicetypes_get_devicelist(dev->devicetype);
    ASSERT_NOT_NULL(lst);
    for (uint32_t i = 0; i < arraylist_count(lst); i++) {
        struct device* d = (struct device*)arraylist_get(lst, i);
        ASSERT_NOT_NULL(d);
        if (0 == strcmp(d->name, dev->name)) {
            arraylist_remove(lst, i);
            return;
        }
    }
}

uint16_t deviceregistry_devicecount() {
    uint16_t ret = 0;
    for (uint16_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        struct arraylist* lst = devicetypes_get_devicelist(i);
        if (0 != lst) {
            ret = ret + arraylist_count(lst);
        }
    }
    return ret;
}

uint16_t deviceregistry_devicecount_type(deviceType dt) {
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            return arraylist_count(lst);
        }
        return 0;
    } else {
        panic("Invalid device type passed to deviceregistry_devicecount_type");
    }
}

struct device* deviceregistry_get_device(deviceType dt, uint16_t idx) {
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            return arraylist_get(lst, idx);
        } else {
            panic("there are no devices of type");
        }
    } else {
        panic("Invalid device type passed to deviceregistry_get_device");
    }
}

void deviceregistry_iterate(DeviceIterator deviceIterator) {
    if (0 != deviceIterator) {
        for (uint32_t i = 0; i < MAX_DEVICE_TYPES; i++) {
            struct arraylist* lst = devicetypes_get_devicelist(i);
            if (0 != lst) {
                for (uint32_t j = 0; j < arraylist_count(lst); j++) {
                    struct device* dev = (struct device*)arraylist_get(lst, j);
                    if (0 != dev) {
                        (*deviceIterator)(dev);
                    } else {
                        panic("null dev in deviceregistry_iterate");
                    }
                }
            }
        }
    } else {
        panic("Invalid iterator passed to deviceregistry_iterate");
    }
}

void deviceregistry_iterate_type(deviceType dt, DeviceIterator deviceIterator) {
    ASSERT_NOT_NULL(deviceIterator);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct device* dev = (struct device*)arraylist_get(lst, j);
                if (0 != dev) {
                    (*deviceIterator)(dev);
                } else {
                    panic("null dev in deviceregistry_iterate");
                }
            }
        }
    } else {
        panic("Invalid deviceType passed to deviceregistry_iterate");
    }
}

void deviceregistry_find_devices_by_description(deviceType dt, const int8_t* description, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(description);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct device* dev = (struct device*)arraylist_get(lst, j);
                if (0 != dev) {
                    if (strcmp(dev->description, description) == 0) {
                        (*cb)(dev);
                    }
                } else {
                    panic("null dev in deviceregistry_iterate");
                }
            }
        }
    } else {
        panic("Invalid deviceType passed to devicemgr_find_devices_by_description");
    }
}

void deviceregistry_find_devices_by_devicetype(deviceType dt, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct device* dev = (struct device*)arraylist_get(lst, j);
                if (0 != dev) {
                    (*cb)(dev);
                } else {
                    panic("null dev in deviceregistry_iterate");
                }
            }
        }
    } else {
        panic("Invalid deviceType passed to devicemgr_find_devices_by_description");
    }
}

/*
 * find device by name.  return zero if there is no such device
 */
struct device* deviceregistry_findDevice(const int8_t* name) {
    ASSERT_NOT_NULL(name);
    for (uint16_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        struct arraylist* lst = devicetypes_get_devicelist(i);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct device* dev = (struct device*)arraylist_get(lst, j);
                if (0 != dev) {
                    if (0 == strcmp(name, dev->name)) {
                        return dev;
                    }
                } else {
                    panic("null dev in deviceregistry_findDevice");
                }
            }
        }
    }
    return 0;
}
