//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/isadma/isadma_page.h>

uint16_t isadma_address_to_page(uint64_t address) {
    return address >> 16;
}

uint16_t isadma_address_to_buffer(uint64_t address) {
    uint16_t page = isadma_address_to_page(address);
	return address - (page << 16);
}
