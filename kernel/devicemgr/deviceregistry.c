//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <devicemgr/deviceregistry.h>
#include <collection/list/list.h>
#include <devicemgr/devicetypes.h>
#include <panic/panic.h>
#include <console/console.h>

void deviceregistry_init() {
    kprintf("Init Device Registry\n");
    devicetypes_init();
}

void deviceregistry_registerdevice(struct device* dev) {
    if (0!=dev){
        if (0!=dev->devicetype) {
        struct list* lst = devicetypes_get_devicelist(dev->devicetype);
        if (0==lst){
            lst = list_new();
            devicetypes_set_devicelist(dev->devicetype, lst);
        }
        list_add(lst, dev);
        } else {
            panic("dev passed to deviceregistry_registerdevice has no type");
        }
    } else {
        panic("Invalid dev passed to deviceregistry_registerdevice");
    }
}

uint16_t deviceregistry_devicecount() {
    uint16_t ret=0;
    for (uint16_t i=0; i<MAX_DEVICE_TYPES;i++) {
        struct list* lst = devicetypes_get_devicelist(i);
        if (0!=lst){
            ret = ret + list_count(lst);
        }
    }
    return ret;
}


uint16_t deviceregistry_devicecount_type(deviceType dt) {
    if ((dt>=0) && (dt<MAX_DEVICE_TYPES)){
        struct list* lst = devicetypes_get_devicelist(dt);
        if (0!=lst){
            return list_count(lst);
        }
        return 0;
    } else {
        panic("Invalid device type passed to deviceregistry_devicecount_type");
    }
}

struct device* deviceregistry_get_device(deviceType dt, uint16_t idx) {
    if ((dt>=0) && (dt<MAX_DEVICE_TYPES)){
        struct list* lst = devicetypes_get_devicelist(dt);
        if (0!=lst){
            return list_get(lst, idx);
        } else {
            panic("there are no devices of type");
        }
    } else {
        panic("Invalid device type passed to deviceregistry_get_device");
    }
}

void deviceregistry_iterate(DeviceIterator deviceIterator) {
    if (0!=deviceIterator){
        for (uint16_t i=0; i<MAX_DEVICE_TYPES;i++) {
            struct list* lst = devicetypes_get_devicelist(i);
            if (0!=lst){
                for (uint16_t j=0; j<list_count(lst);j++) {
                    struct device* dev = (struct device*) list_get(lst, j);
                    if (0!=dev){
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
    if ((dt>=0) && (dt<MAX_DEVICE_TYPES)){
        if (0!=deviceIterator){
            struct list* lst = devicetypes_get_devicelist(dt);
            if (0!=lst){
                for (uint16_t j=0; j<list_count(lst);j++) {
                    struct device* dev = (struct device*) list_get(lst, j);
                    if (0!=dev){
                        (*deviceIterator)(dev);
                    } else {
                        panic("null dev in deviceregistry_iterate");    
                    }
                }
            }
         } else {
            panic("Invalid iterator passed to deviceregistry_iterate");    
        }    
    } else {
        panic("Invalid iterator passed to deviceregistry_iterate");    
    }
}




