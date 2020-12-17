/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>

uint32_t pci_config_address_build(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t enabled){
	uint32_t r;
	
	r = (((uint32_t)enabled) << 31) & 0x80000000;
	r |= (((uint32_t)bus) << 16) & 0x00FF0000;
	r |= (((uint32_t)device) << 11) & 0x0000F800;
	r |= (((uint32_t)function) << 8) & 0x00000700;
	r |= ((uint32_t)offset) & 0x000000FF;
	
	return r;
}
