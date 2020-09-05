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

void ata_interrupt_enable(uint8_t controller, uint8_t channel, bool enabled){
	
	// to clarify, because this may look backwards: yes, the correct thing to do is to clear bit 1 to enable IRQs, and set it to disable
	ata_register_write(controller, channel, ATA_REGISTER_CONTROL, 0x08 | (enabled ? 0 : 2));	// bit 3 is specified as reserved and set to 1, so we have to make sure we don't zero it
	
	return;
}

#endif