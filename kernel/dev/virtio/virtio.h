//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VIRTIO_H
#define _VIRTIO_H

#include <types.h>

// https://wiki.osdev.org/Virtio

// Virtio Manifacturer ID
#define VIRTIO_PCI_MANUFACTURER 0x1AF4

// PCI Virtual Device IDs
#define VIRTIO_PCI_DEVICED_NETWORK 0x1000
#define VIRTIO_PCI_DEVICED_BLOCK 0x1001
#define VIRTIO_PCI_DEVICED_BALLOON 0x1002
#define VIRTIO_PCI_DEVICED_CONSOLE 0x1003
#define VIRTIO_PCI_DEVICED_SCSCI 0x1004
#define VIRTIO_PCI_DEVICED_ENTROPY 0x1005
#define VIRTIO_PCI_DEVICED_9P 0x1009

// generic virtio registers
#define VIRTIO_DEVICE_FEATURES 0x00
#define VIRTIO_GUEST_FEATURES 0x04
#define VIRTIO_QUEUE_ADDRESS 0x08
#define VIRTIO_QUEUE_SIZE 0x0C
#define VIRTIO_QUEUE_SELECT 0x0E
#define VIRTIO_QUEUE_NOTIFY 0x10
#define VIRTIO_DEVICE_STATUS 0x12
#define VIRTIO_ISR_STATUS 0x13

// generic status flags
#define VIRTIO_STATUS_RESET_DEVICE 0 /* reset by writing 0 to the status register. */
#define VIRTIO_STATUS_DEVICE_ACKNOWLEGED 0x01
#define VIRTIO_STATUS_DRIVER_LOADED 0x02
#define VIRTIO_STATUS_DRIVER_READY 0x04
#define VIRTIO_STATUS_FEATURES_OK 0x08
#define VIRTIO_STATUS_DEVICE_ERROR 0x40
#define VIRTIO_STATUS_DRIVER_FAILED 0x80

void virtio_devicemgr_register_devices();

bool virtio_isAligned(uint64_t address, uint32_t alignment);

#endif