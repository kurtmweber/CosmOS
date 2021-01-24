//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/console/vga_console.h>
#include <dev/i386/serial/serial.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_console.h>
#include <sys/deviceapi/deviceapi_vga.h>
#include <sys/kmalloc/kmalloc.h>

#define CONSOLE_TAB_WIDTH 5

struct vga_console_devicedata {
    struct device* vga_device;
    uint16_t vga_console_x_width;
    uint16_t vga_console_y_height;
    uint8_t vga_console_xpos;
    uint8_t vga_console_ypos;
};

/*
 * perform device instance specific init here
 */
void vga_console_dev_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct vga_console_devicedata* deviceData = (struct vga_console_devicedata*)dev->deviceData;
    kprintf("Init %s on %s (%s)\n", dev->description, deviceData->vga_device->name, dev->name);

    deviceData->vga_console_xpos = 0;
    deviceData->vga_console_ypos = 0;

    struct deviceapi_vga* vga_api = (struct deviceapi_vga*)deviceData->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    (*vga_api->query_resolution)(deviceData->vga_device, &(deviceData->vga_console_x_width),
                                 &(deviceData->vga_console_y_height));
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
void vga_console_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);

    struct vga_console_devicedata* deviceData = (struct vga_console_devicedata*)dev->deviceData;
    kprintf("Uninit %s on %s (%s)\n", dev->description, deviceData->vga_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->deviceData);
}

uint8_t vga_console_dev_setpos(struct device* dev, uint8_t x, uint8_t y) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct vga_console_devicedata* deviceData = (struct vga_console_devicedata*)dev->deviceData;
    struct deviceapi_vga* vga_api = (struct deviceapi_vga*)deviceData->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    // error if position is out of range
    if ((x >= deviceData->vga_console_x_width) || (y >= deviceData->vga_console_y_height)) {
        return 0;
    }

    deviceData->vga_console_xpos = x;
    deviceData->vga_console_ypos = y;

    return 1;
}

uint8_t vga_console_dev_write(struct device* dev, const char* c) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct vga_console_devicedata* deviceData = (struct vga_console_devicedata*)dev->deviceData;
    struct deviceapi_vga* vga_api = (struct deviceapi_vga*)deviceData->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    uint64_t i =
        0;  // just in case...it's on the stack anyway, so it's a single-digit # of extra bytes that's gone once we return
    char s[2];

    s[1] = '\0';

    while (c[i]) {
        if (c[i] == '\n') {
            deviceData->vga_console_xpos = 0;
            deviceData->vga_console_ypos++;
            i++;

            continue;
        } else if (c[i] == '\t') {
            deviceData->vga_console_xpos += (CONSOLE_TAB_WIDTH - (deviceData->vga_console_xpos % CONSOLE_TAB_WIDTH));

            if (deviceData->vga_console_xpos >= deviceData->vga_console_x_width) {
                deviceData->vga_console_xpos = 0;
                deviceData->vga_console_ypos++;
            }

            i++;

            continue;
        }

        if ((deviceData->vga_console_xpos >=
             deviceData
                 ->vga_console_x_width)) {  // width is 1-based, pos is 0-based, so if pos = width then we're past the last column
            deviceData->vga_console_xpos = 0;
            deviceData->vga_console_ypos++;
        }

        if (deviceData->vga_console_ypos >= deviceData->vga_console_y_height) {
            // video_scroll_text();
            (*vga_api->scroll_text)(deviceData->vga_device);

            deviceData->vga_console_ypos =
                (deviceData->vga_console_y_height - 1);  // again, ypos is a 0-based index, while height is a quantity
        }

        s[0] = c[i];

        //  video_write_text(s, deviceData->vga_console_ypos, deviceData->vga_console_xpos, NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);
        (*vga_api->write_text)(deviceData->vga_device, s, deviceData->vga_console_ypos, deviceData->vga_console_xpos,
                               NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);

        deviceData->vga_console_xpos++;
        i++;
    }

    return 1;
}

struct device* vga_console_attach(struct device* serial_device) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &vga_console_dev_init;
    deviceinstance->uninit = &vga_console_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = CONSOLE;
    devicemgr_set_device_description(deviceinstance, "VGA Console");
    /*
     * the device api
     */
    struct deviceapi_console* api = (struct deviceapi_console*)kmalloc(sizeof(struct deviceapi_console));
    api->setpos = &vga_console_dev_setpos;
    api->write = &vga_console_dev_write;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct vga_console_devicedata* deviceData =
        (struct vga_console_devicedata*)kmalloc(sizeof(struct vga_console_devicedata));
    deviceData->vga_device = serial_device;
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_attach_device(deviceinstance);

    /*
     * return device
     */
    return deviceinstance;
}

void vga_console_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}