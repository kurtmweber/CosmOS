/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/console/console.h>

uint16_t cursor_position;

void cursor_set_position(uint16_t loc){
	asm_out_b(CRT_INDEX_REGISTER, CURSOR_LOCATION_HIBYTE);
	asm_out_b(CRT_DATA_REGISTER, (uint8_t)((loc >> 8) & 0xFF));
	
	asm_out_b(CRT_INDEX_REGISTER, CURSOR_LOCATION_LOBYTE);
	asm_out_b(CRT_DATA_REGISTER, (uint8_t)(loc & 0xFF));
}
