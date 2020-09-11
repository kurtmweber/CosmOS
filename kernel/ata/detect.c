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
#include <timing/timing.h>

void ata_detect_atapi(uint8_t controller, uint8_t channel){
	ata_register_write(controller, channel, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY_PACKET);
	
	sleep_wait(1);
	
	return;
}

void ata_detect_devices(uint8_t controller){
	uint8_t i, j;
	uint16_t k;
	uint8_t status;
	char *identify_buf;
	
	for (i = 0; i < 2; i++){
		for (j = 0; j < 2; j++){
			ata_register_write(controller, i, ATA_REGISTER_DEVICE_SELECT, 0xA0 | (j << 4));
			sleep_wait(1);
			
			ata_register_write(controller, i, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY);
			sleep_wait(1);
			
			status = ata_register_read(controller, i, ATA_REGISTER_STATUS);
			if (!status){		// no device found
				continue;
			}
			
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
		}
	}
	
	return;
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