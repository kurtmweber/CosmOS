//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICEMGR_H
#define _DEVICEMGR_H

#include <types.h>

#define DEVICEMGR_MAX_DESCRIPTION 64

// forward declare these
struct pci_device;
struct device;

typedef void (*deviceInit)(struct device* dev);
typedef void (*deviceUnInit)(struct device* dev);

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
    PARALLEL = 0x16,
    BDA = 0x17,
    EBDA = 0x18,
    SWAP = 0x19,
    FILESYSTEM = 0x1A,
    PARTITION_TABLE = 0x1B
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
     * un-init function
     */
    deviceUnInit uninit;
    /*
     * human readable description provided by the driver
     */
    uint8_t description[DEVICEMGR_MAX_DESCRIPTION];
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

// attach a device (non-fixed devices... like RAM disks and SWAP)
void devicemgr_attach_device(struct device* dev);

// detach a device (non-fixed devices... like RAM disks and SWAP)
// be aware, this has the effect of deleting the dev struct!
void devicemgr_detach_device(struct device* dev);

#endif