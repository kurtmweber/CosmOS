//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEVICE_H
#define _DEVICE_H
#include <types.h>

#define DEVICE_MAX_DESCRIPTION 64

// forward declare these
struct pci_device;
struct device;

typedef uint8_t (*deviceInit)(struct device* dev);
typedef uint8_t (*deviceUnInit)(struct device* dev);

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
    PARTITION = 0x1D,        // parition0, deviceapi_block
    NULL0 = 0x1E,            // null0, deviceapi_null
    TICK = 0x1F,             // tick0, deviceapi_tick
    RAND = 0x20,             // rand0, deviceapi_rand
    IP = 0x21,               // ip0, deviceapi_ip
    TCP = 0x22,              // tcp0, deviceapi_tcp
    ARP = 0x23,              // arp0, deviceapi_arp
    UDP = 0x24,              // udp0, deviceapi_udp
    ICMP = 0x25              // icmp0, deviceapi_icmp
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
    uint8_t description[DEVICE_MAX_DESCRIPTION];
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

#endif