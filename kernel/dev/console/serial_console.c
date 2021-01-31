//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/console/serial_console.h>
#include <dev/x86-64/serial/serial.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_console.h>
#include <sys/deviceapi/deviceapi_serial.h>
#include <sys/kmalloc/kmalloc.h>

struct serial_console_devicedata {
    struct device* serial_device;
};

/*
 * perform device instance specific init here
 */
uint8_t serial_console_dev_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct serial_console_devicedata* deviceData = (struct serial_console_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->serial_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t serial_console_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);

    struct serial_console_devicedata* deviceData = (struct serial_console_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->serial_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
    return 1;
}

uint8_t serial_console_setpos(struct device* dev, uint8_t x, uint8_t y) {
    // do nothing
    return 0;
}

void serial_console_dev_write(struct device* dev, const char* s) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct serial_console_devicedata* deviceData = (struct serial_console_devicedata*)dev->deviceData;

    struct deviceapi_serial* serial_api = (struct deviceapi_serial*)deviceData->serial_device->api;
    (*serial_api->write)(deviceData->serial_device, s);
}

struct device* serial_console_attach(struct device* serial_device) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &serial_console_dev_init;
    deviceinstance->uninit = &serial_console_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = CONSOLE;
    devicemgr_set_device_description(deviceinstance, "Serial Console");
    /*
     * the device api
     */
    struct deviceapi_console* api = (struct deviceapi_console*)kmalloc(sizeof(struct deviceapi_console));
    api->setpos = &serial_console_setpos;
    api->write = &serial_console_dev_write;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct serial_console_devicedata* deviceData =
        (struct serial_console_devicedata*)kmalloc(sizeof(struct serial_console_devicedata));
    deviceData->serial_device = serial_device;
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(deviceData);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void serial_console_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}