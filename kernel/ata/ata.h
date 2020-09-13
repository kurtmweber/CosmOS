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

#define ATA_DEVICE(x, y, z) (ide_controllers[x].channels[y].devices[z])
#define ATA_SECTORS(x) (x / 512)

typedef enum ata_commands {
	ATA_COMMAND_IDENTIFY_PACKET = 0xA1,
	ATA_COMMAND_IDENTIFY = 0xEC
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

typedef enum ata_identify_offsets {
	// specification gives offsets in 16-bit words, but we're treating it as a char buffer so we're using byte offsets
	ATA_IDENTIFY_OFFSET_GENERAL = 0,
	ATA_IDENTIFY_OFFSET_SERIAL = 20,
	ATA_IDENTIFY_OFFSET_MODEL = 54,
	ATA_IDENTIFY_OFFSET_LBA = 120,
	ATA_IDENTIFY_OFFSET_COMMAND_SET_2 = 166,
	ATA_IDENTIFY_OFFSET_LBA_EXT = 200
} ata_identify_offsets;

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

typedef struct ata_device_t{
	bool exists;
	const char *model;
	bool removable;
	const char *serial;
	uint64_t size;
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

#ifndef _ATA_DETECT_C
void ata_detect_devices(uint8_t controller);
#else
#define CUR_ATA ide_controllers[controller].channels[i].devices[j]

void ata_detect_atapi(uint8_t controller, uint8_t channel);
uint32_t ata_detect_extract_dword(char *identify_buf, ata_identify_offsets offset);
uint64_t ata_detect_extract_qword(char *identify_buf, ata_identify_offsets offset);
char *ata_detect_extract_string(char *identify_buf, uint8_t len, ata_identify_offsets offset);
uint16_t ata_detect_extract_word(char *identify_buf, ata_identify_offsets offset);
char *ata_detect_read_identify(uint8_t controller, uint8_t channel);
#endif

#ifndef _ATA_REGISTERS_C
uint8_t ata_register_read(uint8_t controller, uint8_t channel, ata_registers reg);
uint16_t ata_register_read_dword(uint8_t controller, uint8_t channel, ata_registers reg);
uint16_t ata_register_read_word(uint8_t controller, uint8_t channel, ata_registers reg);
void ata_register_write(uint8_t controller, uint8_t channel, ata_registers reg, uint8_t value);
#else
uint16_t ata_register_port_number(uint8_t controller, uint8_t channel, ata_registers reg);
#endif

#endif