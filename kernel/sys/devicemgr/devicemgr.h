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
    SERIAL = 0x01,           // serial0, deviceapi_serial
    VGA = 0x02,              // vga0, deviceapi_vga
    RTC = 0x03,              // rtc0, deviceapi_rtc
    KEYBOARD = 0x04,         // keyboard0, deviceapi_keyboard
    NIC = 0x05,              // nic0, deviceapi_ethernet
    BRIDGE = 0x06,           // bridge0, deviceapi_bridge
    USB = 0x07,              // usb0, deviceapi_usb
    ATA = 0x08,              // ata0
    PIC = 0x09,              // pic0, deviceapi_pic
    MOUSE = 0x0A,            // mouse0, deviceapi_mouse
    FLOPPY = 0x0B,           // floppy0, deviceapi_floppy
    SPEAKER = 0x0C,          // speaker0, deviceapi_speaker
    PIT = 0x0D,              // pit0, deviceapi_pit
    DSP = 0x0E,              // dsp0, deviceapi_dsp
    CMOS = 0x0F,             // cmos0, deviceapi_cmos
    DMA = 0x10,              // dma0
    CPU = 0x11,              // cpu0, deviceapi_cpu
    RAMDISK = 0x12,          // rd0, deviceapi_block
    VNIC = 0x13,             // vnic0, deviceapi_ethernet
    VBLOCK = 0x14,           // vblock0, deviceapi_block
    DISK = 0x15,             // disk0, deviceapi_block
    PARALLEL = 0x16,         // parallel0, deviceapi_parallel
    BDA = 0x17,              // bda0
    EBDA = 0x18,             // ebda0
    SWAP = 0x19,             // swap0, deviceapi_swap
    FILESYSTEM = 0x1A,       // fs0, deviceapi_filesytem
    PARTITION_TABLE = 0x1B,  // pt0, deviceapi_part_table
    CONSOLE = 0x1C,          // console0, deviceapi_console
    PARTITION = 0x1D         // parition0, deviceapi_parition
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