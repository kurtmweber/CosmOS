/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

/*
* this is a disk device, such as an HD or a CDROM
*/
#ifndef _ATA_DISK_H
#define _ATA_DISK_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>
#include <dev/i386/ata/ata_controller.h>

/*
* a disk only needs one function; a registration called by the controller
*/
void ata_register_disk(struct device* controllerDevice, uint8_t channel, uint8_t disk);

#endif