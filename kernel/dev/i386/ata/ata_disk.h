/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/


#ifndef _ATA_DISK_H
#define _ATA_DISK_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>

void ata_register_disk(struct device* controller, uint8_t channel, uint8_t device);

#endif