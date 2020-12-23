//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VIRTIO_H
#define _VIRTIO_H

#include <types.h>

// https://wiki.osdev.org/Virtio

// generic virtio flags
#define VIRTIO_DEVICE_FEATURES      0x00
#define VIRTIO_GUEST_FEATURES       0x04
#define VIRTIO_QUEUE_ADDRESS        0x08
#define VIRTIO_QUEUE_SIZE           0x0C
#define VIRTIO_QUEUE_SELECT         0x0E
#define VIRTIO_QUEUE_NOTIFY         0x10
#define VIRTIO_DEVICE_STATUS        0x12
#define VIRTIO_ISR_STATUS           0x13

void virtio_devicemgr_register_devices();

#endif