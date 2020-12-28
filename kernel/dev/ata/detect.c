/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <dev/ata/ata.h>
#include <console/console.h>
#include <mm/mm.h>
#include <panic/assert.h>
#include <string/string.h>
#include <sleep/sleep.h>

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
		ATA_CHANNEL(controller, i).selected_device = ATA_DRIVE_SELECT_NONE;
		for (j = 0; j < 2; j++){
			ata_select_device(controller, i, j);
			
			ata_register_write(controller, i, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY);
			sleep_wait(1);
			
			status = ata_register_read(controller, i, ATA_REGISTER_STATUS);
			if (!status){		// no device found
				ATA_CONTROLLER(controller)->channels[i].devices[j].exists = false;
				continue;
			}
			
			// set the exists flag to true
			ATA_CONTROLLER(controller)->channels[i].devices[j].exists = true;
			
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
			
			CUR_ATA.bytes_per_sector = ata_detect_sector_size(identify_buf);
			
			kprintf("   ata%hu.%hu: %s #%s, %s, %llu bytes\n", i, j, strtrim(CUR_ATA.model), strtrim(CUR_ATA.serial), CUR_ATA.removable ? "removable" : "fixed", CUR_ATA.size);
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

uint32_t ata_detect_sector_size(char *identify_buf){
	uint16_t plss_word, version_word;
	
	version_word = ata_detect_extract_word(identify_buf, ATA_IDENTIFY_OFFSET_MAJOR_VERSION);
	if (!(version_word & (1 << 7))){		// if major version is not 7 or greater (signified by bit 7 being set), then the device definitely does not support large sectors
		return 512;
	}
	
	plss_word = ata_detect_extract_word(identify_buf, ATA_IDENTIFY_OFFSET_PLSS);
	
	if ((plss_word & 0x4000) != 0x4000){	// if bit 15 = 0 and bit 14 = 1, then the word contains valid data; if it doesn't, then we know the device does not support large sectors, which is an optional feature in versions 7+
		return 512;
	}
	
	if (!(plss_word & (1 << 12))){		// bit 12 being set signifies support for logical sector greater than 256 words (512 bytes), so if it's not set then we know it's 512
		return 512;
	}
	
	// If we reach this point then we know that the device supports logical sectors > 512 bytes, so we check to see what it is
	
	return ata_detect_extract_dword(identify_buf, ATA_IDENTIFY_OFFSET_SECTOR_SIZE);
}
