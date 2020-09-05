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

#define IDE_SERIAL_IRQ		14

// ATA registers
typedef enum ata_registers {
	ATA_REGISTER_DATA,
	ATA_REGISTER_ERROR,	// read-only
	ATA_REGISTER_FEATURES,	// write-only
	ATA_REGISTER_SECTOR_COUNT_0,
	ATA_REGISTER_LBA_0,
	ATA_REGISTER_LBA_1,
	ATA_REGISTER_LBA_2,
	ATA_REGISTER_DEVICE_SELECT,
	ATA_REGISTER_COMMAND,	// write-only
	ATA_REGISTER_STATUS,	// read-only
	ATA_REGISTER_SECTOR_COUNT_1,
	ATA_REGISTER_LBA_3,
	ATA_REGISTER_LBA_4,
	ATA_REGISTER_LBA_5,
	ATA_REGISTER_CONTROL,
	ATA_REGISTER_ALT_STATUS,
	ATA_REGISTER_DEVICE_ADDRESS
} ata_registers;

typedef struct ata_device_t{
	bool exists;
} ata_device_t;

typedef struct ide_channel_t{
	uint16_t base_io;
	uint16_t base_io_ctrl;
	ata_device_t devices[2];
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

void ata_detect_addresses(uint16_t num_ide);
uint16_t ata_scan_ide_controllers();
void ata_setup_irq(uint16_t num_ide);
#endif

#ifndef _ATA_CONTROL_C
void ata_interrupt_enable(uint8_t controller, uint8_t channel, bool enabled);
#endif

#ifndef _ATA_REGISTERS_C
void ata_register_write(uint8_t controller, uint8_t channel, ata_registers reg, uint8_t value);
#endif

#endif