/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_DETECT_C
#define _ATA_DETECT_C

#include <types.h>
#include <ata/ata.h>
#include <console/console.h>
#include <mm/mm.h>
#include <panic/panic.h>
#include <string/string.h>
#include <timing/timing.h>

void ata_detect_atapi(uint8_t controller, uint8_t channel){
	ata_register_write(controller, channel, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY_PACKET);
	
	sleep_wait(1);
	
	return;
}

void ata_detect_devices(uint8_t controller){
	uint8_t i, j;
	uint8_t status;
	char *identify_buf;
	ata_device_t *tmp;
	
	for (i = 0; i < 2; i++){
		for (j = 0; j < 2; j++){
			ata_register_write(controller, i, ATA_REGISTER_DEVICE_SELECT, 0xA0 | (j << 4));
			sleep_wait(1);
			
			ata_register_write(controller, i, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY);
			sleep_wait(1);
			
			status = ata_register_read(controller, i, ATA_REGISTER_STATUS);
			if (!status){		// no device found
				ide_controllers[controller].channels[i].devices[j].exists = false;
				continue;
			}
			
			// set the exists flag to true
			ide_controllers[controller].channels[i].devices[j].exists = true;
			
			// status isn't 0, so the device exists--so we poll until ready
			while (1){
				status = ata_register_read(controller, i, ATA_REGISTER_STATUS);
				
				if (status & ATA_STATUS_ERROR){
					// do ATAPI identify, it requires a packet request
					ata_detect_atapi(controller, i);
					break;
				} else if (!(status & ATA_STATUS_BUSY) && (status & ATA_STATUS_DRQ)){
					break;
				}
			}
			
			identify_buf = ata_detect_read_identify(controller, i);
			
			if (!(CUR_ATA.serial = ata_detect_extract_string(identify_buf, 20, ATA_IDENTIFY_OFFSET_SERIAL))){
				panic("Invalid length specified for ATA serial number field!");
			}
			
			if (!(CUR_ATA.model = ata_detect_extract_string(identify_buf, 40, ATA_IDENTIFY_OFFSET_MODEL))){
				panic("Invalid length specified for ATA model field!");
			}
			
			
			if (ata_detect_extract_word(identify_buf, ATA_IDENTIFY_OFFSET_COMMAND_SET_2) & (1 << 10)){
				CUR_ATA.size = ata_detect_extract_dword(identify_buf, ATA_IDENTIFY_OFFSET_LBA) * 512;
			} else {
				CUR_ATA.size = ata_detect_extract_qword(identify_buf, ATA_IDENTIFY_OFFSET_LBA_EXT) * 512;
			}
			
			CUR_ATA.removable = (ata_detect_extract_word(identify_buf, ATA_IDENTIFY_OFFSET_GENERAL) & (1 << 7)) >> 7;
			
			kprintf("ata%hu.%hu: %s #%s, %s, %llu bytes\n", i, j, strtrim(CUR_ATA.model), strtrim(CUR_ATA.serial), CUR_ATA.removable ? "removable" : "fixed", CUR_ATA.size);
		}
	}
	
	return;
}

uint32_t ata_detect_extract_dword(char *identify_buf, ata_identify_offsets offset){
	return *((uint32_t *)(&identify_buf[offset]));
}

uint64_t ata_detect_extract_qword(char *identify_buf, ata_identify_offsets offset){
	return *((uint64_t *)(&identify_buf[offset]));
}

char *ata_detect_extract_string(char *identify_buf, uint8_t len, ata_identify_offsets offset){
	char *c;
	uint8_t i;
	
	if (len % 2){
		return (char *)0;
	}
	
	c = (char *)kmalloc((len + 1) * sizeof(char));
	
	for (i = 0; i < (len / 2); i++){
		// for whatever reason, ATA IDENTIFY character string data is big-endian words, so we've got to flip every set of two bytes
		c[(2 * i)] = identify_buf[(2 * i) + offset + 1];
		c[(2 * i) + 1] = identify_buf[(2 * i) + offset];
	}
	
	c[len] = '\0';
	
	return c;
}

uint16_t ata_detect_extract_word(char *identify_buf, ata_identify_offsets offset){
	return *((uint16_t *)(&identify_buf[offset]));
}

char *ata_detect_read_identify(uint8_t controller, uint8_t channel){
	uint16_t *buf;
	
	uint16_t i;
	
	buf = (uint16_t *)kmalloc(512);	// we request 512 bytes, but keep in mind that it's a uint16 so it's a 256-element array
	
	for (i = 0; i < 256; i++){
		buf[i] = ata_register_read_word(controller, channel, ATA_REGISTER_DATA);
	}
	
	return (char *)buf;
}

#endif