//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEMGR_H
#define _DEVICEMGR_H

#include <types.h>

// forward declare these
struct pci_device;
struct device;

typedef void (*deviceInit)(struct device* dev);

typedef enum deviceType {
    NONE = 0x00,
    SERIAL = 0x01,
    VGA = 0x02,
    RTC = 0x03,
    KEYBOARD = 0x04,
    NIC = 0x05,  // uses NIC API
    BRIDGE = 0x06,
    USB = 0x07,
    ATA = 0x08,  // ATA controller
    PIC = 0x09,
    MOUSE = 0x0A,
    FLOPPY = 0x0B,
    SPEAKER = 0x0C,
    PIT = 0x0D,
    DSP = 0x0E,
    CMOS = 0x0F,
    DMA = 0x10,
    CPU = 0x11,
    RAMDISK = 0x12,  // uses Block API
    VNIC = 0x13,     // uses NIC API
    VBLOCK = 0x14,   // uses Block API
    DISK = 0x15,     // uses Block API
    PARALLEL = 0x16
} deviceType;

/*
 * array of names, indexed by deviceType
 */
extern int8_t* DeviceTypeNames[];

typedef struct device {
    /*
     * the combination of name (from DeviceTypeNames) and index
     * create the device name.  ie "serial0".
     */
    int8_t* name;
    uint8_t type_index;
    /*
     * the type (SERIAL, VGA etc)
     */
    enum deviceType devicetype;
    /*
     * init function
     */
    deviceInit init;
    /*
     * human readable description provided by the driver
     */
    int8_t* description;
    /*
     * device-specific data
     */
    void* deviceData;
    /*
     * For PCI devices, this is a struct pci_device*.
     */
    struct pci_device* pci;
    /*
     * pointer to the type-specific API struct
     */
    void* api;
} device_t;

// new device, allocated on the kernel heap
struct device* devicemgr_new_device();

// register a device
void devicemgr_register_device(struct device* dev);

// init the device registry
void devicemgr_init();

// register all the devices
void devicemgr_register_devices();

// count of device instances
uint16_t devicemgr_device_count();

// init all devices
void devicemgr_init_devices();

// set description
void devicemgr_set_device_description(struct device* dev, int8_t* description);

// find a device
struct device* devicemgr_find_device(const int8_t* name);
#endif