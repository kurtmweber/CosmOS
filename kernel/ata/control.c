/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_CONTROL_C
#define _ATA_CONTROL_C

#include <types.h>
#include <ata/ata.h>
#include <console/console.h>
#include <timing/sleep.h>

bool ata_channel_ready(uint8_t controller, uint8_t channel){
	BYTE status;

	status = ata_register_read(controller, channel, ATA_REGISTER_ALT_STATUS);
	if (status & ATA_STATUS_BUSY){		// BSY set (bit 7)
		return false;
	}

	if ((status & ATA_STATUS_ERROR) || (status & ATA_STATUS_WRITE_FAULT)){
		return false;
	}

	if (status & ATA_STATUS_DRIVE_READY){
		return true;
	}

	return false;
}

void ata_interrupt_enable(uint8_t controller, uint8_t channel, bool enabled){
	
	// to clarify, because this may look backwards: yes, the correct thing to do is to clear bit 1 to enable IRQs, and set it to disable
	ata_register_write(controller, channel, ATA_REGISTER_CONTROL, 0x08 | (enabled ? 0 : 2));	// bit 3 is specified as reserved and set to 1, so we have to make sure we don't zero it
	
	return;
}

bool ata_select_device(uint8_t controller, uint8_t channel, ata_drive_selector device){
	BYTE status;
		
	if (ATA_CHANNEL(controller, channel).selected_device == device){
		return true;
	}
	
	status = ata_register_read(controller, channel, ATA_REGISTER_STATUS);
	// bit 7 of status register indicates busy--if it's set then nothing else matters, if not then we check bit 3
	// which indicates ready for data, and we can't change the device on the channel if the already-selected device is waiting for data
	if ((status & ATA_STATUS_BUSY) || (status & ATA_STATUS_DRQ)){
		return false;
	}
	
	ATA_CHANNEL(controller, channel).selected_device = device;
	
	ata_register_write(controller, channel, ATA_REGISTER_DEVICE_SELECT, 0xA0 | (device << 4));
	sleep_wait(1);
	
	return true;
}

#endif