//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/virtio/vblock/vblock.h>
#include <dev/virtio/virtio.h>
#include <dev/virtio/vnic/vnic.h>

void virtio_devicemgr_register_devices() {
    vblock_devicemgr_register_devices();
    vnic_devicemgr_register_devices();
}

bool virtio_isAligned(uint64_t address, uint32_t alignment) {
    return ((address / alignment) * alignment) == address;
}