/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_REGISTERS_C
#define _ATA_REGISTERS_C

#include <types.h>
#include <asm/asm.h>
#include <ata/ata.h>
#include <console/console.h>
#include <panic/panic.h>

void ata_register_write(uint8_t controller, uint8_t channel, ata_registers reg, uint8_t value){
	uint16_t port_prim_base = 0, port_prim_ctrl = 0, port_sec_base = 0, port_sec_ctrl = 0;
	uint16_t port_base = 0, port_ctrl = 0;
	uint16_t out_port_base = 0, out_offset = 0;
	
	port_prim_base = ide_controllers[controller].channels[IDE_CHANNEL_PRIMARY].base_io;
	port_prim_ctrl = ide_controllers[controller].channels[IDE_CHANNEL_PRIMARY].base_io_ctrl;
	port_sec_base = ide_controllers[controller].channels[IDE_CHANNEL_SECONDARY].base_io;
	port_sec_ctrl = ide_controllers[controller].channels[IDE_CHANNEL_SECONDARY].base_io_ctrl;
	
	if (channel == IDE_CHANNEL_PRIMARY){
		port_base = port_prim_base;
		port_ctrl = port_prim_ctrl;
	} else {
		port_base = port_sec_base;
		port_ctrl = port_sec_ctrl;
	}
	
	switch (reg){
		// fallthrough is desired behavior
		case ATA_REGISTER_DATA:
		case ATA_REGISTER_ERROR:
		case ATA_REGISTER_FEATURES:
		case ATA_REGISTER_SECTOR_COUNT_0:
		case ATA_REGISTER_LBA_0:
		case ATA_REGISTER_LBA_1:
		case ATA_REGISTER_LBA_2:
		case ATA_REGISTER_DEVICE_SELECT:
		case ATA_REGISTER_COMMAND:
		case ATA_REGISTER_STATUS:
		case ATA_REGISTER_SECTOR_COUNT_1:
		case ATA_REGISTER_LBA_3:
		case ATA_REGISTER_LBA_4:
		case ATA_REGISTER_LBA_5:
			out_port_base = port_base;
			break;
		case ATA_REGISTER_CONTROL:
		case ATA_REGISTER_ALT_STATUS:
		case ATA_REGISTER_DEVICE_ADDRESS:
			out_port_base = port_ctrl;
			break;
		default:
			panic("Invalid or unsupported ATA register selected!");
			break;
	}
	
	switch (reg){
		// again, fallthrough is intentional
		case ATA_REGISTER_DATA:
			out_offset = 0;
			break;
		case ATA_REGISTER_ERROR:
		case ATA_REGISTER_FEATURES:
			out_offset = 1;
			break;
		case ATA_REGISTER_SECTOR_COUNT_0:
		case ATA_REGISTER_SECTOR_COUNT_1:
			out_offset = 2;
			break;
		case ATA_REGISTER_LBA_0:
		case ATA_REGISTER_LBA_3:
			out_offset = 3;
			break;
		case ATA_REGISTER_LBA_1:
		case ATA_REGISTER_LBA_4:
			out_offset = 4;
			break;
		case ATA_REGISTER_LBA_2:
		case ATA_REGISTER_LBA_5:
			out_offset = 5;
			break;
		case ATA_REGISTER_DEVICE_SELECT:
			out_offset = 6;
			break;
		case ATA_REGISTER_COMMAND:
		case ATA_REGISTER_STATUS:
			out_offset = 7;
			break;
		case ATA_REGISTER_CONTROL:
		case ATA_REGISTER_ALT_STATUS:
			out_offset = 0;
			break;
		case ATA_REGISTER_DEVICE_ADDRESS:
			out_offset = 1;
			break;
		default:
			panic("Invalid or unsupported ATA register selected!");
			break;
	}
	
	//kprintf("Preparing to write value %#X to port %#X\n", value, out_port_base + out_offset);	
	asm_out_b(out_port_base + out_offset, value);
	
	return;	
}

#endif