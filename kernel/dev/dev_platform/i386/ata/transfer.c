/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <dev/dev_platform/i386/ata/ata.h>

BYTE *ata_read(uint64_t start, uint64_t end, uint8_t controller, uint8_t channel, uint8_t device){
	ata_select_device(controller, channel, device);
	return 0;
}
