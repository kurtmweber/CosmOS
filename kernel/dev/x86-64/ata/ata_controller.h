/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_CONTROLLER_H
#define _ATA_CONTROLLER_H

#include <dev/x86-64/ata/ata.h>
#include <sys/devicemgr/devicemgr.h>
#include <types.h>

/*
 * device parameters for an ATA controller
 */
struct ata_device {
    bool exists;
    const char* model;
    bool removable;
    const char* serial;
    uint64_t size;
    uint32_t bytes_per_sector;
    const char* identity;
};

struct ide_channel {
    uint16_t base_io;
    uint16_t base_io_ctrl;
    struct ata_device devices[2];
    ata_drive_selector selected_device;
    uint64_t dma_bus_master_register;
};

struct ata_controller {
    struct ide_channel channels[2];
};

/*
 * basically an ATA controller has two functions. It can register the disks, and it can get a disk struct.
 */
void ata_devicemgr_register_devices();
struct ata_device* ata_get_disk(struct device* dev, uint8_t channel, uint8_t disk);

#endif