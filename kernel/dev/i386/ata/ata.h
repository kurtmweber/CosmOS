/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_H
#define _ATA_H

// https://wiki.osdev.org/PCI_IDE_Controller

typedef enum ata_commands {
	ATA_CMD_READ_PIO = 0x20,
	ATA_CMD_READ_PIO_EXT = 0x24,
	ATA_CMD_READ_DMA = 0xC8,
	ATA_CMD_READ_DMA_EXT = 0x25,
	ATA_CMD_WRITE_PIO = 0x30,
	ATA_CMD_WRITE_PIO_EXT = 0x34,
	ATA_CMD_WRITE_DMA = 0xCA,
	ATA_CMD_WRITE_DMA_EXT = 0x35,
	ATA_CMD_CACHE_FLUSH = 0xE7,
	ATA_CMD_CACHE_FLUSH_EXT = 0xEA,
	ATA_CMD_PACKET = 0xA0,
	ATA_COMMAND_IDENTIFY_PACKET = 0xA1,
	ATA_COMMAND_IDENTIFY = 0xEC,
	ATAPI_CMD_READ = 0xA8,
	ATAPI_CMD_EJECT = 0x1B
} ata_commands;

typedef enum ata_errors {
	ATA_ERROR_NO_ADDRESS_MARK = 0x01,
	ATA_ERROR_NO_TRACK_0 = 0x02,
	ATA_ERROR_ABORT = 0x04,
	ATA_ERROR_MEDIA_CHANGE_REQUEST = 0x08,
	ATA_ERROR_NO_ID_MARK = 0x10,
	ATA_ERROR_MEDIA_CHANGED = 0x20,
	ATA_ERROR_UNCORRECTABLE = 0x40,
	ATA_ERROR_BAD_BLOCK = 0x80
} ata_errors;

typedef enum ata_type {
	IDE_ATA = 0x00,
	IDE_ATAPI = 0x01	
} ata_type;

typedef enum ata_master_slave {
	ATA_MASTER = 0x00,
	ATA_SLAVE = 0x01	
} ata_master_slave;

typedef enum ata_primary_secondary {
	ATA_PRIMARY = 0x00,
	ATA_SECONDARY = 0x01	
} ata_primary_secondary;

typedef enum ata_direction {
	ATA_READ = 0x00,
	ATA_WRITE = 0x01	
} ata_direction;

typedef enum ata_status {
	ATA_STATUS_ERROR = 0x01,
	ATA_STATUS_INDEX = 0x02,
	ATA_STATUS_CORRECTED_DATA = 0x04,
	ATA_STATUS_DRQ = 0x08,
	ATA_STATUS_DSC = 0x10,
	ATA_STATUS_WRITE_FAULT = 0x20,
	ATA_STATUS_DRIVE_READY = 0x40,
	ATA_STATUS_BUSY = 0x80
} ata_status;

typedef enum ata_identify_offsets {
	// specification gives offsets in 16-bit words, but we're treating it as a char buffer so we're using byte offsets
	ATA_IDENTIFY_OFFSET_GENERAL = 0,
	ATA_IDENTIFY_OFFSET_SERIAL = 20,
	ATA_IDENTIFY_OFFSET_MODEL = 54,
	ATA_IDENTIFY_OFFSET_LBA = 120,
	ATA_IDENTIFY_OFFSET_MAJOR_VERSION = 160,
	ATA_IDENTIFY_OFFSET_COMMAND_SET_2 = 166,
	ATA_IDENTIFY_OFFSET_LBA_EXT = 200,
	ATA_IDENTIFY_OFFSET_PLSS = 212,		// Physical Sector Size / Logical Sector Size
	ATA_IDENTIFY_OFFSET_SECTOR_SIZE = 234
} ata_identify_offsets;

typedef enum ata_drive_selector {
	ATA_DRIVE_SELECT_0 = 0,
	ATA_DRIVE_SELECT_1 = 1,
	ATA_DRIVE_SELECT_NONE = 2
} ata_drive_selector;

typedef enum ata_registers {
	ATA_REGISTER_DATA,
	ATA_REGISTER_ERROR,	// read-only
	ATA_REGISTER_FEATURES,	// write-only
	ATA_REGISTER_SECTOR_COUNT_0,
	ATA_REGISTER_LBA_0,
	ATA_REGISTER_LBA_1,
	ATA_REGISTER_LBA_2,
	ATA_devicemgr_register_device_SELECT,
	ATA_REGISTER_COMMAND,	// write-only
	ATA_REGISTER_STATUS,	// read-only
	ATA_REGISTER_SECTOR_COUNT_1,
	ATA_REGISTER_LBA_3,
	ATA_REGISTER_LBA_4,
	ATA_REGISTER_LBA_5,
	ATA_REGISTER_CONTROL,
	ATA_REGISTER_ALT_STATUS,
	ATA_devicemgr_register_device_ADDRESS
} ata_registers;

#endif