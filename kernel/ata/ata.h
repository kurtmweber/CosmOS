/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_H
#define _ATA_H

#include <pci/devicetree.h>

#define IDE_CHANNEL_PRIMARY	0
#define IDE_CHANNEL_SECONDARY	1

typedef struct ide_channel_t{
	uint16_t base_io;
	uint16_t base_io_ctrl;
} ide_channel_t;

typedef struct ide_controller_t{
	pci_device_t *pci;
	ide_channel_t channels[2];
} ide_controller_t;

#ifndef _ATA_C
extern ide_controller_t *ide_controllers;

void ata_init();
#else
ide_controller_t *ide_controllers;
#endif

#endif